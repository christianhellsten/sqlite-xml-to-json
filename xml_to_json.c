#include "xml_to_json.h"

// Returns true if string only contains whitespace
bool is_blank(const char *str) {
  while (*str) {
    if (!isspace((unsigned char)*str)) {  // Cast to unsigned char for safe isspace usage
      return false;
    }
    str++;
  }
  return true;
}

json_object* convert_xml_string_to_json(const char *xml_content) {
  if (xml_content == NULL) return NULL;

  xmlDoc *doc = xmlReadMemory(xml_content, strlen(xml_content), NULL, NULL, 0);
  if (doc == NULL) return NULL;

  xmlNode *root_element = xmlDocGetRootElement(doc);
  if (root_element == NULL) {
    xmlFreeDoc(doc);
    return NULL;
  }

  json_object* json_obj = convert_xml_to_json(root_element);

  xmlFreeDoc(doc);
  return json_obj;
}

json_object* convert_xml_to_json(xmlNode *root) {
  if (root == NULL) return NULL;

  json_object *root_json = json_object_new_object();
  const char *root_name = (const char *)root->name;
  json_object *root_element_json = json_object_new_object();

  json_object_object_add(root_json, root_name, root_element_json);
  convert_attributes_to_json(root_element_json, root);
  convert_children_to_json(root_element_json, root->children);

  return root_json;
}

void convert_children_to_json(json_object *parent_json, xmlNode *node) {
  if (node == NULL) return;

  for (xmlNode *child = node; child != NULL; child = child->next) {
    if (child->type == XML_ELEMENT_NODE) {
      const char *child_name = (const char *)child->name;
      json_object *child_json = json_object_new_object();
      convert_attributes_to_json(child_json, child);
      if (child->children != NULL && child->children->type == XML_TEXT_NODE) {
        const char *text_content = (const char *)child->children->content;
        if (!is_blank(text_content)) {
          json_object_object_add(parent_json, child_name, json_object_new_string(text_content));
          continue;
        }
      }
      json_object *existing_child_json = json_object_object_get(parent_json, child_name);
      if (existing_child_json != NULL) {
        if (!json_object_is_type(existing_child_json, json_type_array)) {
          json_object *child_array = json_object_new_array();
          json_object_array_add(child_array, existing_child_json);
          json_object_object_del(parent_json, child_name);
          json_object_object_add(parent_json, child_name, child_array);
          existing_child_json = child_array;
        }
        json_object_array_add(existing_child_json, child_json);
      } else {
        json_object_object_add(parent_json, child_name, child_json);
      }
      convert_children_to_json(child_json, child->children);
    }
  }
}

void convert_attributes_to_json(json_object *parent_json, xmlNode *node) {
  if (node == NULL) return;

  for (xmlAttr *attr = node->properties; attr != NULL; attr = attr->next) {
    const char *attr_name = (const char *)attr->name;
    const char *attr_value = (const char *)xmlGetProp(node, attr->name);
    const char *attr_ns = (attr->ns && attr->ns->prefix) ? (const char *)attr->ns->prefix : "";

    char attr_with_prefix_and_ns[strlen(attr_name) + strlen(attr_ns) + 3];  // Sufficient buffer size for name, namespace, and '@'
    if (strlen(attr_ns) > 0) {
      snprintf(attr_with_prefix_and_ns, sizeof(attr_with_prefix_and_ns), "@%s:%s", attr_ns, attr_name);
    } else {
      snprintf(attr_with_prefix_and_ns, sizeof(attr_with_prefix_and_ns), "@%s", attr_name);
    }

    json_object_object_add(parent_json, attr_with_prefix_and_ns, json_object_new_string(attr_value));

    xmlFree((void *)attr_value);  // Free the memory allocated by xmlGetProp
  }
}
