/*
 * Xournal++
 *
 * This small program extracts a preview out of a xoj file
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GPL
 */

// Set to true to write a log with errors and debug logs to /tmp/xojtmb.log
#define DEBUG_THUMBNAILER false


#include <algorithm>
#include <fstream>
#include <iostream>

#include <config-paths.h>
#include <config.h>

#include "XojPreviewExtractor.h"
#include "i18n.h"
using std::cerr;
using std::cout;
using std::endl;
#include <cairo-svg.h>
#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>

void initLocalisation() {
#ifdef ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    textdomain(GETTEXT_PACKAGE);
#endif  // ENABLE_NLS

    std::locale::global(std::locale(""));  //"" - system default locale
    std::cout.imbue(std::locale());
}

void logMessage(string msg, bool error) {
    if (error) {
        cerr << msg << endl;
    } else {
        cout << msg << endl;
    }

#if DEBUG_THUMBNAILER
    std::ofstream ofs;
    ofs.open("/tmp/xojtmb.log", std::ofstream::out | std::ofstream::app);

    if (error) {
        ofs << "E: ";
    } else {
        ofs << "I: ";
    }

    ofs << msg << endl;

    ofs.close();
#endif
}

int main(int argc, char* argv[]) {
    initLocalisation();

    // check args count
    if (argc != 3) {
        logMessage(_("xoj-preview-extractor: call with INPUT.xoj OUTPUT.png"), true);
        return 1;
    }

    XojPreviewExtractor extractor;
    PreviewExtractResult result = extractor.readFile(argv[1]);

    switch (result) {
        case PREVIEW_RESULT_IMAGE_READ:
            // continue to write preview
            break;

        case PREVIEW_RESULT_BAD_FILE_EXTENSION:
            logMessage((_F("xoj-preview-extractor: file \"{1}\" is not .xoj file") % argv[1]).str(), true);
            return 2;

        case PREVIEW_RESULT_COULD_NOT_OPEN_FILE:
            logMessage((_F("xoj-preview-extractor: opening input file \"{1}\" failed") % argv[1]).str(), true);
            return 3;

        case PREVIEW_RESULT_NO_PREVIEW:
            logMessage((_F("xoj-preview-extractor: file \"{1}\" contains no preview") % argv[1]).str(), true);
            return 4;

        case PREVIEW_RESULT_ERROR_READING_PREVIEW:
        default:
            logMessage(_("xoj-preview-extractor: no preview and page found, maybe an invalid file?"), true);
            return 5;
    }


    gsize dataLen = 0;
    unsigned char* imageData = extractor.getData(dataLen);

    // The following code is for rendering the Xournal++ icon on top of thumbnails.

    // Cairo was unable to load the image, so fallback to writing the PNG data to disk.
    FILE* fp = fopen(argv[2], "wb");
    if (!fp) {
        logMessage((_F("xoj-preview-extractor: opening output file \"{1}\" failed") % argv[2]).str(), true);
        return 6;
    }
    fwrite(imageData, dataLen, 1, fp);
    fclose(fp);

    logMessage(_("xoj-preview-extractor: successfully extracted"), false);
    return 0;
}
