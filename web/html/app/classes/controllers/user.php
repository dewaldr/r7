<?php
/**
 * user.php
 *
 * @package default
 */


namespace controllers;

class User extends Base {

	public function __construct() {
		parent::__construct();
	}


	static public function login($f3, $params) {

		if (User::isLoggedIn($f3, $params)) {
			$f3->reroute('/');
			return;
		}

		$login = $f3->get('POST.login');
		$f3->scrub($login);

		$password = $f3->get('POST.password');
		$f3->scrub($password);
		$hashed = hash('sha256', $password);

		if (!empty($login) && !empty($password)) {
			$db = \Registry::get('db');
			$mapper = new \DB\SQL\Mapper($db, 'user');
			$auth = new \Auth($mapper, array('id'=>'login', 'pw'=>'password'));

			// User login success
			if ($auth->login($login, $hashed)) {

				$f3->set('SESSION.login', $login);
				$f3->set('SESSION.language', 'en'); // or 'af'
				$f3->set('SESSION.start_page', 'compact');

				// Select user profile start page and language
				// TODO: use mapper
				$rows = $db->exec('SELECT language, start_page FROM user WHERE login = ?',
					$f3->get('SESSION.login'));

				// TODO: Report exception
				if (count($rows) == 1) {
					$f3->set('SESSION.language', $rows[0]['language']);
					$f3->set('SESSION.start_page', $rows[0]['start_page']);
				}

				$f3->reroute('/');
				return;
			}
		}

		// User login fail
		$f3->set('title', 'Robot 7 Login');
		// TODO: display notifications
		$f3->set('SESSION.notifications.error', 'Invalid username or password');
		echo \View::instance()->render('views/user/login.phtml');

		return;
	}


	static public function logout($f3, $params) {

		// Check if the user is logged in
		if (User::isLoggedIn($f3, $params)) {
			$f3->clear('SESSION');
			$f3->reroute('/');
		}
	}


	static public function isLoggedIn($f3, $params) {

		$login = $f3->get('SESSION.login');
		$f3->scrub($login);

		if ($login) {
			return True;
		}
		else {
			return False;
		}
	}


	public function showPwd($f3, $params) {

		// Check if the user is logged in
		if ($f3->get('SESSION.login')) {

			// Reset notifications
			$f3->set('SESSION.notifications.error', NULL);
			// Render password change dialog
			$f3->set('title', 'Robot 7 User');
			echo \View::instance()->render('views/user/pwd.phtml');
		}
		else {
			$f3->reroute('/');
		}
	}


	public function changePwd($f3, $params) {

		// Check if the user is logged in
		if (User::isLoggedIn($f3, $params)) {

			// Reset notifications
			$f3->set('SESSION.notifications.error', NULL);

			// Assume submit button pressed
			// Verify input
			// TODO: additional verification required
			$oldpwd = $f3->get('POST.oldpwd');
			$f3->scrub($oldpw);
			$newpwd = $f3->get('POST.newpwd');
			$f3->scrub($newpwd);

			if (!empty($oldpwd) && !empty($newpwd)) {
				$db = \Registry::get('db');

				// Verify current password
				$res = $db->exec('SELECT count(*) AS isvalid FROM user WHERE user.login = ? AND user.password = ?',
					array(1 => $f3->get('SESSION.login'), 2 => hash('sha256', $oldpwd)));

				if ($res[0]['isvalid'] == 1) {
					// Set new password
					$res = $db->exec('UPDATE user SET password = ? WHERE login = ?',
						array(1 => hash('sha256', $newpwd), 2 => $f3->get('SESSION.login')));
				}
				else {
					// TODO: display error
					//Password change fail
					$f3->set('title', 'Robot 7 User');
					// TODO: display notifications
					$f3->set('SESSION.notifications.error', 'Invalid current password');
					echo \View::instance()->render('views/user/pwd.phtml');

					return;
				}
			}
			else {
				// TODO: display error
				//Password change fail
				$f3->set('title', 'Robot 7 User');
				// TODO: display notifications
				$f3->set('SESSION.notifications.error', 'Invalid password input');
				echo \View::instance()->render('views/user/pwd.phtml');

				return;
			}
		}
		$f3->reroute('/');
	}


    public function showPrefs($f3, $params) {

		// Check if the user is logged in
		if ($f3->get('SESSION.login')) {

			// Reset notifications
			$f3->set('SESSION.notifications.error', NULL);
			// Render password change dialog
			$f3->set('title', 'Robot 7 User');
			echo \View::instance()->render('views/user/prefs.phtml');
		}
		else {
			$f3->reroute('/');
		}
	}

	public function changePrefs($f3, $params) {

		// Check if the user is logged in
		if (User::isLoggedIn($f3, $params)) {

			// Reset notifications
			$f3->set('SESSION.notifications.error', NULL);

			// Assume submit button pressed
			// Verify input
			// TODO: additional verification required
			$language = $f3->get('POST.language');
			$f3->scrub($language);
			$start_page = $f3->get('POST.start_page');
			$f3->scrub($start_page);

			if (!empty($language) && !empty($start_page)) {
				$db = \Registry::get('db');

				$res = $db->exec('UPDATE user SET language = ?, start_page = ? WHERE login = ?',
						array(1 => $language, 2 => $start_page, 3 => $f3->get('SESSION.login')));
			}
			else {
				// TODO: display error
				// Preferences change fail
				$f3->set('title', 'Robot 7 User');
				// TODO: display notifications
				$f3->set('SESSION.notifications.error', 'Invalid preferences input');
				echo \View::instance()->render('views/user/prefs.phtml');

				return;
			}
		}
		$f3->reroute('/');
	}
}
