#include "xml_to_json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read content from standard input
char *read_stdin_content() {
  char *content = NULL;
  char buffer[4096];
  size_t total_size = 0;

  while (1) {
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), stdin);
    if (bytes_read == 0) {
      break;
    }

    // Resize content buffer
    content = (char *)realloc(content, total_size + bytes_read + 1);
    if (content == NULL) {
      printf("Memory allocation failed.\n");
      return NULL;
    }

    // Append buffer to content
    memcpy(content + total_size, buffer, bytes_read);
    total_size += bytes_read;
  }

  // Null-terminate content
  content[total_size] = '\0';

  return content;
}

int main() {
  // Read XML content from stdin
  char *xml_content = read_stdin_content();
  if (xml_content == NULL) {
    return 1;
  }

  // Convert XML content to JSON
  json_object *json = convert_xml_string_to_json(xml_content);

  // Print JSON output
  printf("%s\n", json_object_to_json_string(json));

  // Cleanup
  free(xml_content);
  json_object_put(json);

  return 0;
}
