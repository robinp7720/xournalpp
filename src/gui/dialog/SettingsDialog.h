/*
 * Xournal++
 *
 * Settings Dialog
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include "settingsComponents/SettingsSidebarWidget.h"

#include "control/Control.h"
#include "control/settings/Settings.h"
#include "gui/GladeGui.h"
#include "util/audio/DeviceInfo.h"

#include "DeviceClassConfigGui.h"
#include "LanguageConfigGui.h"
#include "LatexSettingsPanel.h"

class SettingsDialog: public GladeGui {
public:
    SettingsDialog(GladeSearchpath* gladeSearchPath, Settings* settings, Control* control);
    ~SettingsDialog() override;

public:
    void show(GtkWindow* parent) override;



    void save();


private:
    Settings* settings = nullptr;
    Control* control = nullptr;

    SettingsSidebarWidget settingsSidebarWidget;
};
