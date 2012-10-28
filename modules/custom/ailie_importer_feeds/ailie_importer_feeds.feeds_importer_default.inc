<?php
/**
 * @file
 * ailie_importer_feeds.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function ailie_importer_feeds_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'isengine';
  $feeds_importer->config = array(
    'name' => 'isEngine importer',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'IsEngineParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'project',
        'expire' => '-1',
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => 0,
          ),
          1 => array(
            'source' => 'subtitle',
            'target' => 'field_subtitle',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'description',
            'target' => 'body',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'collection_of',
            'target' => 'field_collection_of',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'cost',
            'target' => 'field_cost',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'thank_you',
            'target' => 'field_thank_you',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'is_recent',
            'target' => 'field_is_recent',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'is_series',
            'target' => 'field_is_series',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'is_sold',
            'target' => 'field_is_sold',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'subcategory',
            'target' => 'field_project_type',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'materials',
            'target' => 'field_materials',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'year',
            'target' => 'field_date',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'modified',
            'target' => 'created',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'images',
            'target' => 'field_media:MediaFeedsLibraryProvider',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'filtered_html',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['isengine'] = $feeds_importer;

  return $export;
}
