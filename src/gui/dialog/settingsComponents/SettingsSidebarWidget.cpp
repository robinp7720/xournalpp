//
// Created by robin on 2/23/21.
//

#include "SettingsSidebarWidget.h"

SettingsSidebarWidget::SettingsSidebarWidget(GladeSearchpath* gladeSearchpath):
        GladeGui(gladeSearchpath, "settings/panel-list.ui", "main_listbox")
{

}

void SettingsSidebarWidget::show(GtkWindow* parent) {

}