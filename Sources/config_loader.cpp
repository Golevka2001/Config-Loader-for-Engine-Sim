/******************************************************************************
 * @File: config_loader.cpp
 * @Brief: Main window of the tool.
 * @Author: Gol3vka<gol3vka@163.com>
 * @Version: 2.1.0
 * @Created date: 2022/10/21
 * @Last modified date: 2022/10/31
 *****************************************************************************/

#include "../Includes/config_loader.h"

#include <QDir>
#include <QMessageBox>
#include <QtWidgets/QMainWindow>

#include "../Includes/config.h"
#include "../Includes/selection_dialog.h"

ConfigLoader::ConfigLoader(QWidget *parent) : QMainWindow(parent)
{
  ui.setupUi(this);

  /* set absolute path of folders */
  // assets_folder_path_ = "C:/Program Files/Engine Sim/assets/";
  QDir temp_dir("../assets/");
  assets_folder_path_ = temp_dir.absolutePath() + '/';
  config_.SetRootPath(assets_folder_path_);
  engines_folder_path_ = assets_folder_path_ + "engines/";
  themes_folder_path_ = assets_folder_path_ + "themes/";

  /* check path */
  if (!QDir(assets_folder_path_).exists())
  {
    QMessageBox::critical(NULL, "ERROR", "Can not find folder \"assets\".",
                          QMessageBox::Close);
    exit(EXIT_FAILURE);
  }
  if (!QDir(engines_folder_path_).exists())
  {
    QMessageBox::critical(NULL, "ERROR",
                          "Can not find folder \"assets/engines\".",
                          QMessageBox::Close);
    exit(EXIT_FAILURE);
  }
  if (!QDir(themes_folder_path_).exists())
  {
    QMessageBox::critical(NULL, "ERROR",
                          "Can not find folder \"assets/themes\".",
                          QMessageBox::Close);
    exit(EXIT_FAILURE);
  }

  /* read "main.mr" as default config */
  config_.Read();

  /* display */
  ui.engine_file_name_browser->setText(config_.import_engine_path_);
  ui.theme_file_name_browser->setText(config_.import_theme_path_);

  /* set file filter_ */
  filter_.clear();
  filter_.append("*.mr");

  /* create file system model */
  file_model_ = new QFileSystemModel();
  /* root path and filter_ */
  file_model_->setRootPath(assets_folder_path_);
  file_model_->setNameFilters(filter_);
  file_model_->setNameFilterDisables(false);
  file_model_->setReadOnly(false);

  /* tree view of engine files */
  DisplayTreeView();
}

ConfigLoader::~ConfigLoader() { delete file_model_; }

/* display engine files in tree view */
void ConfigLoader::DisplayTreeView()
{
  /* engine files */
  /* bind file system model to tree view */
  ui.engine_files_tree_view->setAnimated(true);
  ui.engine_files_tree_view->setModel(file_model_);
  ui.engine_files_tree_view->setRootIndex(
      file_model_->index(engines_folder_path_));
  /* hide headers and columns */
  ui.engine_files_tree_view->header()->hide();
  for (int i = 1; i < 4; ++i) ui.engine_files_tree_view->hideColumn(i);

  /* theme files */
  /* bind file system model to tree view */
  ui.theme_files_tree_view->setAnimated(true);
  ui.theme_files_tree_view->setModel(file_model_);
  ui.theme_files_tree_view->setRootIndex(
      file_model_->index(themes_folder_path_));

  /* hide headers and columns */
  ui.theme_files_tree_view->header()->hide();
  for (int i = 1; i < 4; ++i) ui.theme_files_tree_view->hideColumn(i);
}

/* get selected item from engine files tree */
void ConfigLoader::on_engine_files_tree_view_clicked(const QModelIndex &index)
{
  /* get the type of selected item */
  QString type = file_model_->type(index);
  if (type == "File Folder") return;

  /* display the name of selected file */
  config_.import_engine_path_ =
      config_.assets_folder_dir_.relativeFilePath(file_model_->filePath(index));
  ui.engine_file_name_browser->setText(config_.import_engine_path_);
}

/* get selected item from theme files tree */
void ConfigLoader::on_theme_files_tree_view_clicked(const QModelIndex &index)
{
  /* get the type of selected item */
  QString type = file_model_->type(index);
  if (type == "File Folder") return;

  /* display the name of selected file */
  config_.import_theme_path_ =
      config_.assets_folder_dir_.relativeFilePath(file_model_->filePath(index));
  ui.theme_file_name_browser->setText(config_.import_theme_path_);
}

/* apply and write into "main.mr" */
void ConfigLoader::on_apply_button_clicked()
{
  /* ensure an engine file is selected */
  if (config_.import_engine_path_.isEmpty())
  {
    QMessageBox::critical(NULL, "ERROR", "Please select an engine.",
                          QMessageBox::Ok);
    return;
  }
  /* ensure a theme file is selected */
  if (config_.import_theme_path_.isEmpty())
  {
    QMessageBox::critical(NULL, "ERROR", "Please select a theme.",
                          QMessageBox::Ok);
    return;
  }

  if (!config_.ReadEngineFile(assets_folder_path_ +
                              config_.import_engine_path_))
    return;
  if (!config_.ReadThemeFile(assets_folder_path_ + config_.import_theme_path_))
    return;
  config_.BackUp();
  config_.Write();
}

/* quit */
void ConfigLoader::on_quit_button_clicked()
{
  delete file_model_;
  exit(EXIT_SUCCESS);
}
