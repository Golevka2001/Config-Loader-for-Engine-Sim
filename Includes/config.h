/******************************************************************************
 * @File: config.h
 * @Author: Gol3vka<gol3vka@163.com>
 * @Version: 2.0.1
 * @Created date: 2022/10/21
 * @Last modified date: 2022/10/29
 *****************************************************************************/

#pragma once

#include <QDir>
#include <QFile>
#include <QString>

class Config
{
 public:
  Config();
  ~Config();

  /* path of "assets": /.../assets/ */
  void SetRootPath(const QString &root_path);

  /* read "main.mr" as default config */
  void Read();
  /* write into "main.mr" */
  void Write();
  /* back up old "main.mr" */
  void BackUp();

  /* read engine file to get public node */
  bool ReadEngineFile(const QString &full_path);
  /* read theme file to get public node */
  bool ReadThemeFile(const QString &full_path);

  QDir assets_folder_dir_;
  QString import_engine_path_;
  QString import_theme_path_;

 private:
  QFile main_file_;
  QString set_engine_command_;
  QString set_theme_command_;
};
