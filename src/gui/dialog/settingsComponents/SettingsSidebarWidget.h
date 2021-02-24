//
// Created by robin on 2/23/21.
//

#pragma  once


#include <gtk/gtk.h>
#include "gui/GladeGui.h"

class SettingsSidebarWidget: public GladeGui {
public:
    SettingsSidebarWidget(GladeSearchpath*);

public:
    void show(GtkWindow* parent) override;
};