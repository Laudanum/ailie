<?php
/**
 * @file
 * ailie_importer_feeds.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ailie_importer_feeds_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
}
