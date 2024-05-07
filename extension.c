#include "xml_to_json.h"
#include "extension.h"

static void xml_to_json(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (argc != 1) {
        sqlite3_result_error(context, "Wrong number of arguments", -1);
        return;
    }

    const char *xml_input = (const char *)sqlite3_value_text(argv[0]);
    if (!xml_input) {
        sqlite3_result_null(context);
        return;
    }

    json_object *json_obj = convert_xml_string_to_json(xml_input);
    if(json_obj != NULL) {
    const char *json_output = json_object_to_json_string(json_obj);
    sqlite3_result_text(context, json_output, -1, SQLITE_TRANSIENT);
    json_object_put(json_obj);
    }
    else
    {
      // TODO: handle error
    }
}

int sqlite3_extension_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi) {
    // Initialize the SQLite extension
    SQLITE_EXTENSION_INIT2(pApi)

    // Define the function flags
    const int flags = SQLITE_UTF8 | SQLITE_INNOCUOUS;

    // Register the XML to JSON conversion function
    int result = sqlite3_create_function(db, "xml_to_json", 1, SQLITE_UTF8, NULL, xml_to_json, NULL, NULL);
    if (result != SQLITE_OK) {
        *pzErrMsg = sqlite3_mprintf("Failed to register function xml_to_json");
        return result;
    }

    return SQLITE_OK;
}
