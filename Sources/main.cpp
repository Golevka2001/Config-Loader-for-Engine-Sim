/******************************************************************************
 * A config loader with GUI.\
 *   Users select an engine and theme,\
 *   then "main.mr" will be automatically generated.
 * @File: main.cpp
 * @Author: Gol3vka<gol3vka@163.com>
 * @Version: 2.1.0
 * @What's new:
 *   1) Read old config file as default config.
 * @Created date: 2022/10/21
 * @Last modified date: 2022/10/31
 *****************************************************************************/

#include <QtWidgets/QApplication>

#include "../Includes/config_loader.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  ConfigLoader w;
  w.show();
  return a.exec();
}
