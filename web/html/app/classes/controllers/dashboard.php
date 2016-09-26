<?php
/**
 * dashboard.php
 *
 * @package default
 */


namespace controllers;

class Dashboard extends Base {

	public function __construct() {
		parent::__construct();
	}

	public function start($f3, $params) {

		// Check if the user is logged in
		if (User::isLoggedIn($f3, $params)) {
			$this->index($f3, $params);
		}
		else {
			User::login($f3, $params);
		}
	}

	public function index($f3, $params) {

		$f3->set('title', 'Robot 7 Home');

		$db = \Registry::get('db');
		$f3->set('result', $db->exec('SELECT id, name, type, power, phase FROM machine WHERE is_active = 1'));

		echo \View::instance()->render('views/dashboard/index.phtml');
	}
}
