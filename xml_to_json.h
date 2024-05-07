//
// SQLite3 extension that converts XML to JSON
//
// Dependencies:
// - sqlite3
// - libxml
// - json-c
//
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <json.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>  // for isspace

json_object* convert_xml_string_to_json(const char *xml_content);
json_object* convert_xml_to_json(xmlNode *root);

void convert_children_to_json(json_object *parent_json, xmlNode *node);
void convert_attributes_to_json(json_object *parent_json, xmlNode *node);
