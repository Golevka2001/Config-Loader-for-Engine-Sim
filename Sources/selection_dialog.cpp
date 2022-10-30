/******************************************************************************
 * @File: selection_dialog.cpp
 * @Brife: A dialog for users to select engine, transmission and vehicle nodes.
 * @Author: Gol3vka<gol3vka@163.com>
 * @Version: 2.0.1
 * @Created date: 2022/10/21
 * @Last modified date: 2022/10/29
 *****************************************************************************/

#include "../Includes/selection_dialog.h"

#include <QDesktopServices>
#include <QUrl>

SelectionDialog::SelectionDialog(QWidget *parent) : QDialog(parent)
{
  ui.setupUi(this);
  engine_idx_ = 0;
  transmission_idx_ = 0;
  vehicle_idx_ = 0;
}

SelectionDialog::~SelectionDialog() {}

/* set item for combo boxes */
void SelectionDialog::SetItem(const QStringList &pub_nodes)
{
  ui.engine_selector->addItem("--None--");
  ui.transmission_selector->addItem("--None--");
  ui.vehicle_selector->addItem("--None--");

  ui.engine_selector->addItems(pub_nodes);
  ui.transmission_selector->addItems(pub_nodes);
  ui.vehicle_selector->addItems(pub_nodes);
}

/* get user selection */
void SelectionDialog::on_engine_selector_activated(int index)
{
  engine_idx_ = index;
}
void SelectionDialog::on_transmission_selector_activated(int index)
{
  transmission_idx_ = index;
}
void SelectionDialog::on_vehicle_selector_activated(int index)
{
  vehicle_idx_ = index;
}

/* open the file */
void SelectionDialog::on_open_button_clicked()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(file_path_));
}