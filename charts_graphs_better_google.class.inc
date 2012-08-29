<?php

/**
 * @file
 *   Implementation of abstract class ChartsGraphsCanvas for Google Charts library.
 *
 */

/**
 * Implementation of abstract class ChartsGraphsCanvas for Google Charts library.
 */
class ChartsGraphsBetterGoogle extends ChartsGraphsCanvas {

  var $width = 450;
  var $height = 200;
  var $title = '';

  protected function _get_chart_type() {
    switch ($this->type) {
      case 'line':
        $type = 'LineChart';
        break;

      case 'area':
        $type = 'AreaChart';
        break;

      case 'bar':
        $type = 'ColumnChart';
        break;

      case 'pie':
        $type = 'PieChart';
        break;

      case 'side_bar':
        $type = 'BarChart';
        break;

       case 'pie_3d':
         $type = 'PieChart';
         break;
/*
      case 'queued_bar':
        $type = 'bvo';
        break;

       case 'stacked_bar':
         $type = 'bvs';
         break;

       case 'stacked_side_bar':
         $type = 'bhs';
         break;
*/
    }
    return $type;
  }

  protected function _get_chart_data() {
  	$series = $this->series;
  	$x_labels = $this->x_labels;
  	
  	$hdr = "[ 'x'";
  	$ret = "";
  
  	$n_cols=0;
  	foreach($series as $id => $arr) {
  		$hdr .= ", '" . $id . "'";
  		$n_cols = max($n_cols, count($arr));
  	}
  	$hdr .= " ],\n";
  	
  	for($i=0; $i<$n_cols; $i++) {
  		
  		if($i > 0) $ret .= ",\n";
  		
  		$ret .= "['" . $x_labels[$i] . "'";
  		
  		foreach($series as $id => $arr) {
  			$ret .= "," . $arr[$i];
  		}
  		
  		$ret .= "]";
  		
  	}
  	
  	return $hdr . $ret;
  }
  
  protected function _get_chart_options() {
  	
  	$ret = "title: '" . $this->title . "', ";
  	$ret .= "width: '" . $this->width . "px', ";
  	$ret .= "height: '" . $this->height . "px'";
  	
  	return $ret;
  }
    
  /**
   * Function that renders data.
   */
  public function get_chart() {
  	
    $provider = charts_graphs_better_google_chartgraph_provider();
    $chart_id = sprintf(
      '%s-chart-%d',
      $this->type,
      $this->getUnique_ID()
    );
    $alt_text = sprintf(
      '%s %s chart',
      $this->title,
      $provider->chart_types[$this->type]
    );
    
    drupal_set_html_head('<script type="text/javascript" src="https://www.google.com/jsapi" ></script>');
    
    $type = $this->_get_chart_type();
    $data = $this->_get_chart_data();
    $options = $this->_get_chart_options();
    
  	$js = sprintf("google.load('visualization', '1', {packages:['corechart']});
      google.setOnLoadCallback(drawChart);
      function drawChart() {
        var data = google.visualization.arrayToDataTable([
%s
        ]);

        var options = {
%s
        };

        var chart = new google.visualization.%s(document.getElementById('%s-wrapper'));
        chart.draw(data, options);
      }", 
      $data,
      $options,
      $type,
      check_plain($chart_id));
		drupal_add_js($js, 'inline');
		
		$html = '<div id="%s-wrapper" class="charts_graphs_google_chart charts_graphs_google_chart_%s"';
		$html .= ' style="width: ' . $this->width . 'px; height: ' . $this->height . 'px;"';
		$html .= '></div>';

    $output = sprintf(
      $html,
      check_plain($chart_id),
      check_plain($this->type)
    );
    return $output;
  }
}
