#include "sqlite3.h"
#include "sqlite3ext.h"

SQLITE_EXTENSION_INIT1

static void xml_to_json(sqlite3_context *context, int argc, sqlite3_value **argv);
