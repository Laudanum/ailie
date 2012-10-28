<?php
/**
 * @file
 * ailie_exhibitions.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ailie_exhibitions_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ailie_exhibitions_node_info() {
  $items = array(
    'exhibition' => array(
      'name' => t('Exhibition'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
