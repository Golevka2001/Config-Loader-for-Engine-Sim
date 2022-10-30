/******************************************************************************
 * @File: config.cpp
 * @Brief: Config class.
 * @Author: Gol3vka<gol3vka@163.com>
 * @Version: 2.0.1
 * @Created date: 2022/10/21
 * @Last modified date: 2022/10/29
 *****************************************************************************/

#include "../Includes/config.h"

#include <QByteArray>
#include <QDebug>  /////////////////
#include <QIODevice>
#include <QMessageBox>
#include <QRegularExpression>
#include <QStringList>
#include <QTextStream>

#include "../Includes/selection_dialog.h"

Config::Config()
{
  import_theme_path_.clear();
  import_engine_path_.clear();
  set_theme_command_.clear();
  set_engine_command_.clear();
}

Config::~Config() {}

/* path of "assets": /.../assets/ */
void Config::SetRootPath(const QString &root_path)
{
  assets_folder_dir_.setPath(root_path);
  main_file_.setFileName(root_path + "main.mr");
}

/* read "main.mr" as default config */
void Config::Read()
{
  /* check */
  if (!main_file_.exists()) return;
  QByteArray line;
  line.clear();
  QString theme_file_name_;
  if (!main_file_.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QMessageBox::critical(NULL, "ERROR", "File \"main.mr\" is not accessible.",
                          QMessageBox::Ok);
    return;
  }
  else
  {
    QRegularExpression import_theme_re("^import[ |\s]+\"themes/");
    QRegularExpression import_engine_re("^import[ |\s]+\"engines/");
    while (!main_file_.atEnd())
    {
      line = main_file_.readLine();
      if (import_theme_re.match(line).hasMatch())
        import_theme_path_ = line.split('\"')[1];
      if (import_engine_re.match(line).hasMatch())
        import_engine_path_ = line.split('\"')[1];
    }
    main_file_.close();
  }
}

/* write into "main.mr" */
void Config::Write()
{
  if (!main_file_.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    QMessageBox::critical(NULL, "ERROR", "File \"main.mr\" is not accessible.",
                          QMessageBox::Ok);
    return;
  }
  else
  {
    QStringList content;
    content.clear();
    content.append("import \"engine_sim.mr\"");
    /* import theme */
    content.append("\n/********** import theme **********/");
    content.append("import \"" + import_theme_path_ + '\"');
    /* import engine */
    content.append("\n/********** import engine **********/");
    content.append("import \"" + import_engine_path_ + '\"');
    /* set theme & units */
    content.append("\n/********** set theme & units **********/");
    content.append("unit_names units()");
    content.append(set_theme_command_);

    /****************************************************************************
     *
     * TODO: modify units here, use parameters or add member vars.
     *
     ***************************************************************************/

    content.append(")");
    /* set engine */
    content.append("\n/********** set engine **********/");
    content.append(set_engine_command_);

    QTextStream stream(&main_file_);
    for (int i = 0; i < content.size(); ++i) stream << content[i] << "\n";
    main_file_.close();
  }

  /* notify */
  QMessageBox::information(
      NULL, "INFO",
      "Successful!\nOld configuration is backed up to \"main.mr.backup\".",
      QMessageBox::Ok);
}

/* back up old "main.mr" */
void Config::BackUp()
{
  /* remove old backup file */
  QFile backup_file(assets_folder_dir_.path() + "/main.mr.backup");
  if (backup_file.exists())
    if (!backup_file.remove())
      QMessageBox::critical(NULL, "ERROR", backup_file.errorString(),
                            QMessageBox::Ok);
  /* back up "main.mr" to "main.mr.backup" before modifying */
  if (main_file_.exists())
    if (!main_file_.copy(backup_file.fileName()))
      QMessageBox::critical(NULL, "ERROR", main_file_.errorString(),
                            QMessageBox::Ok);
}

/* read engine file to get public node */
bool Config::ReadEngineFile(const QString &file_path)
{
  /* clear */
  set_engine_command_.clear();

  /* get relative path */
  import_engine_path_ = assets_folder_dir_.relativeFilePath(file_path);

  /* check */
  QFile file(file_path);
  if (!file.exists())
  {
    QMessageBox::critical(
        NULL, "ERROR",
        "Can not find engine file \"" + import_engine_path_ + "\".",
        QMessageBox::Ok);
    return false;
  }

  /* open and read the file line-by-line */
  QStringList pub_nodes;
  QByteArray line;
  bool main_node_exists = false;
  pub_nodes.clear();
  line.clear();
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QMessageBox::critical(
        NULL, "ERROR",
        "Engine file \"" + import_engine_path_ + "\" is not accessible.",
        QMessageBox::Ok);
    return false;
  }
  else
  {
    /* search for public nodes */
    QRegularExpression pub_node_re("^public node ");
    QRegularExpression main_node_re("^public node main");
    while (!file.atEnd())
    {
      line = file.readLine();
      /* break if "main" exists */
      if (main_node_re.match(line).hasMatch())
      {
        main_node_exists = true;
        break;
      }
      /* store the name of public node */
      if (pub_node_re.match(line).hasMatch())
        pub_nodes.append(line.split(' ')[2]);
    }
    file.close();
  }

  /* generate set-engine command */
  if (main_node_exists)
    set_engine_command_ = "main()";
  else if (pub_nodes.size() < 1)
  {
    QMessageBox::critical(NULL, "ERROR", "Invalid engine file.",
                          QMessageBox::Ok);
    return false;
  }
  else if (pub_nodes.size() == 1)
    set_engine_command_ = "set_engine(" + pub_nodes[0] + "())";
  else
  {
    /* selection dialog */
    SelectionDialog *dialog = new SelectionDialog;
    dialog->file_path_ = file_path;
    dialog->SetItem(pub_nodes);
    int dialog_ret = dialog->exec();
    if (dialog_ret == QDialog::Accepted)
    {
      if (dialog->engine_idx_ == 0)
      {
        QMessageBox::critical(NULL, "ERROR", "No engine.", QMessageBox::Ok);
        delete dialog;
        return false;
      }
      else
        set_engine_command_ +=
            "set_engine(" + pub_nodes[dialog->engine_idx_ - 1] + "())";
      if (dialog->transmission_idx_ != 0)
        set_engine_command_ += "\nset_transmission(" +
                               pub_nodes[dialog->transmission_idx_ - 1] + "())";
      if (dialog->vehicle_idx_ != 0)
        set_engine_command_ +=
            "\nset_vehicle(" + pub_nodes[dialog->vehicle_idx_ - 1] + "())";
    }
    else
    {
      delete dialog;
      return false;
    }
    delete dialog;
  }
  return true;
}

/* read theme file to get public node */
bool Config::ReadThemeFile(const QString &file_path)
{
  /* clear */
  set_theme_command_.clear();

  /* get relative path */
  import_theme_path_ = assets_folder_dir_.relativeFilePath(file_path);

  /* check */
  QFile file(file_path);
  if (!file.exists())
  {
    QMessageBox::critical(
        NULL, "ERROR",
        "Can not find theme file \"" + import_theme_path_ + "\".",
        QMessageBox::Ok);
    return false;
  }

  /* open and read the file line-by-line */
  QStringList pub_nodes;
  QByteArray line;
  pub_nodes.clear();
  line.clear();
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QMessageBox::critical(
        NULL, "ERROR",
        "Theme file \"" + import_theme_path_ + "\" is not accessible.",
        QMessageBox::Ok);
    return false;
  }
  else
  {
    /* search for public nodes */
    QRegularExpression pub_node_re("^public node ");
    while (!file.atEnd())
    {
      line = file.readLine();
      if (pub_node_re.match(line).hasMatch())
      {
        set_theme_command_ = line.split(' ')[2] + '(';
        break;
      }
    }
    file.close();
  }
  return true;
}