<?php

/**
 */
class FeedsIsEngineParser extends FeedsParser {
  /**
   * Implements FeedsParser::parse().
   */
  protected $content_type = 'work';

  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    error_log("Importing " . $content_type);

    // Set time zone to GMT for parsing dates with strtotime().
    $tz = date_default_timezone_get();
    date_default_timezone_set('GMT');
    // Yes, using a DOM parser is a bit inefficient, but will do for now
    $xml = new SimpleXMLElement($fetcher_result->getRaw());
    $result = new FeedsParserResult();
    foreach ($xml->record as $record) {
//    for ( $n = 0; $n < 6; $n++ ) {
//      $record = $xml->record[$n];
//  we need one parser for each content type so filter on that type
      if ( trim($record->category) == $this->content_type ) {
        $item = array('title' => _decode_html($record->title));
        if ($record['modified']) {
          $item['modified'] = (int)$record['modified'];
        }
        if ($record->subtitle) {
          $item['subtitle'] = _decode_html($record->subtitle->textContent);
        }
        if ($record->id) {
          $item['isengine_id'] = (int) $record->id->textContent;
        }
        if ($record->abstract) {
          $item['abstract'] = _decode_html($record->abstract->textContent);
        }
        if ($record->description) {
          $item['description'] = _decode_html($record->description->textContent);
        }
        if ($record->subcategory) {
          $item['subcategory'] = (string) $record->subcategory->textContent;
        }
        if ($record->is_recent) {
          $item['is_recent'] = 1;
        }
        if ($record->is_series) {
          $item['is_series'] = 1;
        }
        if ($record->collection_of) {
          $item['collection_of'] = _decode_html($record->collection_of->textContent);
        }
        if ($record->cost) {
          $item['cost'] = (string) (string) $record->cost->textContent;
        }
        if ($record->year) {
          $year = $record->year->textContent;
          if ( intval($year) > 3000 ) {
            // convert the integer into epoch and then into %Y
            $year = strftime('%Y', (int)$year);
          }
          $item['year'] = (string) $year;
        }
        if ($record->is_sold) {
          $item['is_sold'] = 1;
        }
        if ($record->thank_you) {
          $item['thank_you'] = _decode_html($record->thank_you->textContent);
        }
        if ($record->media) {
          $item['materials'] = _decode_html($record->media->textContent);
        }
        if (count($record->exhibition)) {
          //  an array of exhibitions with title, venue, start and stop dates
          $exhibitions = array();
          foreach ( $record->gallery as $exhibition ) {
            $title = trim((string)$exhibition->title->textContent);
            //  search for an exhibition with this title
            //  query
            //  SELECT nid FROM node WHERE node.title IS "$title"
          }
        }
        if (count($record->gallery)) {
          //  an array of images with caption, attribution and colour
//          error_log("Gallery items: " . count($record->gallery));
//            $item['geolocations'][$k] = new FeedsGeoTermElement($v);
//  FeedsElement > FeedsTermElement > FeedsGeoTermElement
          //  possibly create and relate media item 
          //  alternatively create imagefield item
          $images = array();
          foreach ( $record->gallery as $image ) {
            $id = intval($record->id);
            $src = preg_replace('/_\d+x\d+/', "", $image->image);
            $src = "public://xfiles/" . $id . "/" . $src;
            $attr = array();
            if ( $image->caption ) {
              $attr['filename'] = trim((string) $image->caption);
            }
            if ( $image->attribution )
              $attr['field_attribution'] = trim((string) $image->attribution);
            if ( $image->colour )
              $attr['field_colour'] = trim((string) $image->colour);
              
            $file = _create_file_entity($src, $attr);
            if ( $file ) {
              error_log("Created file " . ($file->filename));
              $images[] = $file->uri;
            }
          }
          if ( count($images) ) {
            $item['images'] = $images;
            error_log("Added " . count($images) . " images.");
          }
        }

        $result->items[] = $item;
      }
    }
    date_default_timezone_set($tz);
    return $result;
  }

  /**
   * Implements FeedsParser::getMappingSources().
   */
  public function getMappingSources() {
    return array(
      'title' => array(
        'name' => t('Title'),
        'description' => t('URL of the feed item.'),
      ),
      'is_engine_id' => array(
        'name' => t('Legacy ID'),
        'description' => t('The original ID of the record.'),
      ),
      'subtitle' => array(
        'name' => t('Subtitle'),
      ),
      'abstract' => array(
        'name' => t('Abstract'),
      ),
      'description' => array(
        'name' => t('Description'),
      ),
      'subcategory' => array(
        'name' => t('Subcategory'),
      ),
      'modified' => array(
        'name' => t('Date modified'),
      ),
      'images' => array(
        'name' => t('Images'),
        'description' => t('An array of images.')
      ),
    ) + parent::getMappingSources();
  }
}


class FeedsIsEngineWorksParser extends FeedsIsEngineParser {
  protected $content_type = 'work';

  public function getMappingSources() {
    return array(
      'is_recent' => array(
        'name' => t('Is recent'),
      ),
      'is_series' => array(
        'name' => t('Is series'),
      ),
      'collection_of' => array(
        'name' => t('Collection of'),
      ),
      'cost' => array(
        'name' => t('Cost'),
      ),
      'is_sold' => array(
        'name' => t('Is sold'),
      ),
      'thank_you' => array(
        'name' => t('Thank you'),
      ),
      'year' => array(
        'name' => t('Year'),
      ),
      'materials' => array(
        'name' => t('Media'),
        'description' => t('Materials used to create the work.')
      ),
    ) + parent::getMappingSources();
  }
}


class FeedsIsEngineExhibitionsParser extends FeedsIsEngineParser {
  protected $content_type = 'exhibition';

  public function getMappingSources() {
    return array(
      'is_solo' => array(
        'name' => t('Solo?'),
      ),
      'date_preview' => array(
        'name' => t('Preview date'),
      ),
      'date_start' => array(
        'name' => t('Start date'),
      ),
      'date_end' => array(
        'name' => t('End date'),
      ),
      'venue_name' => array(
        'name' => t('Venue name'),
      ),
      'venue_address' => array(
        'name' => t('Venue address'),
      ),
      'venue_link' => array(
        'name' => t('Venue link'),
      ),
      'venue_hours' => array(
        'name' => t('Venue opening hours'),
      ),
      //  @TODO
      //  review, attribution, link (repeating)

      //  quote, quotation, attribution (repeating)

      //  invite image (repeating)

      //  catalog, image, essay, publisher (repeating)
    ) + parent::getMappingSources();
  }
}

//   16 workshops
class FeedsIsEngineWorkshopsParser extends FeedsIsEngineParser {
  protected $content_type = 'workshop';

  public function getMappingSources() {
    return array(
      'outline' => array(
        'name' => t('Workshop outline'),
      ),
      'lesson_plan' => array(
        'name' => t('Lesson plan'),
      ),
      'requirement_list' => array(
        'name' => t('Requirement list'),
      ),
      'skill_level' => array(
        'name' => t('Skill level'),
      ),
      'days' => array(
        'name' => t('Number of days'),
      ),
      'hours_a_day' => array(
        'name' => t('Hours per day'),
      ),
      'cost' => array(
        'name' => t('Cost'),
      ),
      'cost_notes' => array(
        'name' => t('Cost notes'),
      ),
      'max_class_size' => array(
        'name' => t('Maximum class size'),
      ),
      'course_notes' => array(
        'name' => t('Course notes'),
      ),
      'venue_requirements' => array(
        'name' => t('Venue requirements'),
      ),
      'history' => array(
        'name' => t('History'),
      ),
    ) + parent::getMappingSources();
  }
}


class FeedsIsEngineClassesParser extends FeedsIsEngineParser {
  protected $content_type = 'class';

  public function getMappingSources() {
    return array(
      'organisation_name' => array(
        'name' => t('Organising organisation'),
      ),
      'organisation_contact' => array(
        'name' => t('Organising contact'),
      ),
      'organisation_bookings' => array(
        'name' => t('Organising bookings'),
      ),
      'outline' => array(
        'name' => t('Class outline'),
      ),
      'date_start' => array(
        'name' => t('Start date'),
      ),
      'date_stop' => array(
        'name' => t('Stop date'),
      ),
      'venue_name' => array(
        'name' => t('Venue'),
      ),
      'venue_address' => array(
        'name' => t('Venue address'),
      ),
      'venue_country' => array(
        'name' => t('Venue country'),
      ),
    ) + parent::getMappingSources();
  }
}


class FeedsIsEngineNewsParser extends FeedsIsEngineParser {
  protected $content_type = 'news';

  public function getMappingSources() {
    return array(
      'date' => array(
        'name' => t('News date'),
      ),
      'link' => array(
        'name' => t('News link'),
      ),
    ) + parent::getMappingSources();
  }
}


class FeedsIsEngineQuotesParser extends FeedsIsEngineParser {
  protected $content_type = 'quote';

  public function getMappingSources() {
    return array(
      'attribution' => array(
        'name' => t('Quote attribution'),
      ),
      'quote' => array(
        'name' => t('Quotation'),
      ),
      'date' => array(
        'name' => t('Date'),
      ),
    ) + parent::getMappingSources();
  }
}


//  http://drupal.org/node/1438668
function _create_file_entity($src, $attrs=array(), $directory_path="feeds-importer") {
  global $user;
  //  we could check to see if the file already exists.
  $temp_path = drupal_realpath('public://' . $directory_path);
  if ( ! file_exists($temp_path) ) {
    try {
      mkdir($temp_path);
    } catch (Exception $e) {
      drupal_set_message("Could not create directory $directory_path.", 'error');
    }
  }

    // first check that the file exists
  if ( file_exists($src) ) {
        // create a file object for Drupal
    $file = (object) array(
      'uri' => $src,
      'uid' => $user->uid,
      'filemime' => file_get_mimetype($src),
      'status' => 1,
      'display' => 1,
      //'media_display' => 1,
    );    
  
    // now get Drupal to copy it
    //  we need to split the file from the filename
    $basename = basename($src);

    $fileobj = file_copy($file, 'public://' . $directory_path . "/" . $basename);

    // we should have a file object with an id.  The id is all we need
    if ( ! $fileobj )
      return null;
    $fid = $fileobj->fid;

    // load the entity for this file (note, returns an array, indexed on fid
    $file_entities = entity_load('file', array($fid));
//    if ( is_array($file_entities) )

    $file_entity = $file_entities[$fid];

    foreach ($attrs as $key=>$value) {
      try {
        $file_entity->$key = $value;
      } catch (Exception $e) {
      }
    }
    /*
    // not required

    $file_entity->display = 1;
    $file_entity->media_display = 1;
    //  http://stackoverflow.com/questions/12369244/programattically-adding-file-fails-solved
    //  set language
    $file_entity->field_file = array(LANGUAGE_NONE => array('0' => (array)$fileobj));
    */
    entity_save('file', $file_entity);

    //  reset the file name here

    return $file_entity;
  } else {
    error_log("$src does not exist.");
  }
}

function _decode_html($string) {
  return html_entity_decode((string) $string, ENT_COMPAT, 'UTF-8');
}