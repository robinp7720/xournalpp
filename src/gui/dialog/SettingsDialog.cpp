#include "SettingsDialog.h"

#include <utility>

#include <config.h>

#include "gui/widgets/ZoomCallib.h"

#include "ButtonConfigGui.h"
#include "DeviceListHelper.h"
#include "StringUtils.h"
#include "Util.h"
#include "i18n.h"

SettingsDialog::SettingsDialog(GladeSearchpath* gladeSearchPath, Settings* settings, Control* control):
        GladeGui(gladeSearchPath, "settings/settings.ui", "settingsDialog"),
        settings(settings),
        control(control)
{

}

SettingsDialog::~SettingsDialog() {
    for (ButtonConfigGui* bcg: this->buttonConfigs) {
        delete bcg;
    }
    this->buttonConfigs.clear();

    for (DeviceClassConfigGui* dev: this->deviceClassConfigs) {
        delete dev;
    }
    this->deviceClassConfigs.clear();

    // DO NOT delete settings!
    this->settings = nullptr;
}

void SettingsDialog::initLanguageSettings() {
    languageConfig = std::make_unique<LanguageConfigGui>(getGladeSearchPath(), get("hboxLanguageSelect"), settings);
}

void SettingsDialog::initMouseButtonEvents(const char* hbox, int button, bool withDevice) {
    this->buttonConfigs.push_back(new ButtonConfigGui(getGladeSearchPath(), get(hbox), settings, button, withDevice));
}

void SettingsDialog::initMouseButtonEvents() {

}

void SettingsDialog::setDpi(int dpi) {

}

void SettingsDialog::show(GtkWindow* parent) {
    load();

    // detect display size. If large enough, we enlarge the Settings
    // Window up to 1000x740.
    GdkDisplay* disp = gdk_display_get_default();
    if (disp != NULL) {
        GdkWindow* win = gtk_widget_get_window(GTK_WIDGET(parent));
        if (win != NULL) {
            GdkMonitor* moni = gdk_display_get_monitor_at_window(disp, win);
            GdkRectangle workarea;
            gdk_monitor_get_workarea(moni, &workarea);
            gint w = -1;
            gint h = -1;
            if (workarea.width > 1100) {
                w = 1000;
            } else if (workarea.width > 920) {
                w = 900;
            }
            if (workarea.height > 800) {
                h = 740;
            } else if (workarea.height > 620) {
                h = 600;
            }
            gtk_window_set_default_size(GTK_WINDOW(this->window), w, h);
        } else {
            g_message("Parent window does not have a GDK Window. This is unexpected.");
        }
    }

    gtk_window_set_transient_for(GTK_WINDOW(this->window), parent);

    gtk_widget_show(this->window);

}

void SettingsDialog::loadCheckbox(const char* name, gboolean value) {
    GtkToggleButton* b = GTK_TOGGLE_BUTTON(get(name));
    gtk_toggle_button_set_active(b, value);
}

auto SettingsDialog::getCheckbox(const char* name) -> bool {
    GtkToggleButton* b = GTK_TOGGLE_BUTTON(get(name));
    return gtk_toggle_button_get_active(b);
}

void SettingsDialog::loadSlider(const char* name, double value) {
    GtkRange* range = GTK_RANGE(get(name));
    gtk_range_set_value(range, value);
}

auto SettingsDialog::getSlider(const char* name) -> double {
    GtkRange* range = GTK_RANGE(get(name));
    return gtk_range_get_value(range);
}

/**
 * Checkbox was toggled, enable / disable it
 */
void SettingsDialog::enableWithCheckbox(const string& checkboxId, const string& widgetId) {
    GtkWidget* checkboxWidget = get(checkboxId);
    bool enabled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkboxWidget));
    gtk_widget_set_sensitive(get(widgetId), enabled);
}

void SettingsDialog::disableWithCheckbox(const string& checkboxId, const string& widgetId) {
    GtkWidget* checkboxWidget = get(checkboxId);
    bool enabled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkboxWidget));
    gtk_widget_set_sensitive(get(widgetId), !enabled);
}

void SettingsDialog::updatePressureSensitivityOptions() {

}

void SettingsDialog::customHandRecognitionToggled() {
    GtkWidget* cbTouchDisableMethod = get("cbTouchDisableMethod");
    int touchMethod = gtk_combo_box_get_active(GTK_COMBO_BOX(cbTouchDisableMethod));
    gtk_widget_set_sensitive(get("boxCustomTouchDisableSettings"), touchMethod == 2);
}

void SettingsDialog::customStylusIconTypeChanged() {
    GtkWidget* cbStylusCursorType = get("cbStylusCursorType");
    int stylusCursorType = gtk_combo_box_get_active(GTK_COMBO_BOX(cbStylusCursorType));
    bool showCursorHighlightOptions = stylusCursorType != STYLUS_CURSOR_NONE;
    gtk_widget_set_sensitive(get("highlightCursorGrid"), showCursorHighlightOptions);
}

void SettingsDialog::load() {

}

auto SettingsDialog::updateHideString(const string& hidden, bool hideMenubar, bool hideSidebar) -> string {
    string newHidden;

    for (const string& element: StringUtils::split(hidden, ',')) {
        if (element == "mainMenubar") {
            if (hideMenubar) {
                hideMenubar = false;
            } else {
                continue;
            }
        } else if (element == "sidebarContents") {
            if (hideSidebar) {
                hideSidebar = false;
            } else {
                continue;
            }
        }

        if (!newHidden.empty()) {
            newHidden += ",";
        }
        newHidden += element;
    }

    if (hideMenubar) {
        if (!newHidden.empty()) {
            newHidden += ",";
        }
        newHidden += "mainMenubar";
    }

    if (hideSidebar) {
        if (!newHidden.empty()) {
            newHidden += ",";
        }
        newHidden += "sidebarContents";
    }

    return newHidden;
}

void SettingsDialog::save() {

}
