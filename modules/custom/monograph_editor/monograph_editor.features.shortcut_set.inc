<?php
/**
 * @file
 * monograph_editor.features.shortcut_set.inc
 */

/**
 * Implements hook_shortcut_default_shortcut_set().
 */
function monograph_editor_shortcut_default_shortcut_set() {
  $shortcut_sets = array();
  $shortcut_sets['shortcut-set-3'] = array(
    'set_name' => 'shortcut-set-3',
    'title' => 'Editor',
  );
  return $shortcut_sets;
}