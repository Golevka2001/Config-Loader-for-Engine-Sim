/******************************************************************************
 * @File: selection_dialog.h
 * @Author: Gol3vka<gol3vka@163.com>
 * @Version: 2.0.1
 * @Created date: 2022/10/21
 * @Last modified date: 2022/10/29
 *****************************************************************************/

#pragma once

#include <QString>
#include <QStringList>

#include "ui_selection_dialog.h"

class SelectionDialog : public QDialog
{
  Q_OBJECT

 public:
  SelectionDialog(QWidget *parent = nullptr);
  ~SelectionDialog();

  /* set item for combo boxes */
  void SetItem(const QStringList &pub_nodes);

  /* file path */
  QString file_path_;

  /* user selection */
  int engine_idx_;
  int transmission_idx_;
  int vehicle_idx_;

 private slots:
  /* get user selection */
  void on_engine_selector_activated(int index);
  void on_transmission_selector_activated(int index);
  void on_vehicle_selector_activated(int index);

  /* open the file */
  void on_open_button_clicked();

 private:
  Ui::SelectionDialogClass ui;
};
