#include "SettingsDialog.h"

#include <utility>

#include <config.h>

#include "gui/GladeSearchpath.h"
#include "gui/widgets/ZoomCallib.h"

#include "ButtonConfigGui.h"
#include "DeviceListHelper.h"
#include "StringUtils.h"
#include "Util.h"
#include "i18n.h"
#include "../GladeGui.h"

SettingsDialog::SettingsDialog(GladeSearchpath* gladeSearchPath, Settings* settings, Control* control):
        GladeGui(gladeSearchPath, "settings/settings.ui", "settingsDialog"),
        settings(settings),
        control(control),
        settingsSidebarWidget(gladeSearchPath)
{
    gtk_container_add(GTK_CONTAINER(this->get("sidePanel")), this->settingsSidebarWidget.getWindow());

    auto test = gtk_builder_new();

    GError* error = nullptr;

    auto filepath = gladeSearchPath->findFile("", "settings/pages/saveload.ui");

    gtk_builder_add_from_file(test, filepath.u8string().c_str(), &error);

    auto page = gtk_builder_get_object(test, "page");

    gtk_container_add(GTK_CONTAINER(this->get("settingsPanel")), GTK_WIDGET(page));

}

SettingsDialog::~SettingsDialog() {

}


void SettingsDialog::show(GtkWindow* parent) {

    gtk_window_set_transient_for(GTK_WINDOW(this->window), parent);

    gtk_widget_show(this->window);

}



void SettingsDialog::save() {

}
