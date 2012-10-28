<?php
/**
 * @file
 * ailie_works.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ailie_works_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ailie_works_node_info() {
  $items = array(
    'project' => array(
      'name' => t('Work'),
      'base' => 'node_content',
      'description' => t('This is the default type for all records about works of art or text.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
