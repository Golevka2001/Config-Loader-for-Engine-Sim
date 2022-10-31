/******************************************************************************
 * @File: config_loader.h
 * @Author: Gol3vka<gol3vka@163.com>
 * @Version: 2.1.0
 * @Created date: 2022/10/21
 * @Last modified date: 2022/10/31
 *****************************************************************************/

#pragma once

#include <QFileSystemModel>
#include <QModelIndex>
#include <QString>

#include "../Includes/config.h"
#include "ui_config_loader.h"

class ConfigLoader : public QMainWindow
{
  Q_OBJECT

 public:
  ConfigLoader(QWidget *parent = nullptr);
  ~ConfigLoader();

  /* display engine/theme files in tree view */
  void DisplayTreeView();

 private slots:
  /* get selected item from engine files tree */
  void on_engine_files_tree_view_clicked(const QModelIndex &index);
  /* get selected item from theme files tree */
  void on_theme_files_tree_view_clicked(const QModelIndex &index);

  /* apply and write into "main.mr" */
  void on_apply_button_clicked();
  /* quit */
  void on_quit_button_clicked();

 private:
  Ui::ConfigLoaderClass ui;

  QFileSystemModel *file_model_;
  QStringList filter_;

  Config config_;

  QString assets_folder_path_;
  QString engines_folder_path_;
  QString themes_folder_path_;
};
