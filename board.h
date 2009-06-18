/***********************************************************************
 *
 * Copyright (C) 2007-2008 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef BOARD_H
#define BOARD_H

#include <QTime>
#include <QWidget>
#include "AQCode.h" // ADDED BY LARS PETTER MOSTAD
class QLabel;
class QMainWindow;
class QTimeLine;
class QTimer;
class Maze;
class Theme;

class Board : public QWidget
{
	Q_OBJECT
public:
	Board(QMainWindow* parent);
	~Board();

signals:
	void pauseChecked(bool checked);
	void pauseAvailable(bool run);
	void finished(int seconds, int steps, int algorithm, int size);

public slots:
	void newGame();
	void loadGame();
	void saveGame();
	void pauseGame(bool paused);
	void loadSettings();

protected:
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void mousePressEvent(); // ADDED BY TERJE GUNDERSEN
	virtual void keyReleaseEvent(QKeyEvent* event); // ADDED BY LARS PETTER MOSTAD
	virtual void paintEvent(QPaintEvent*);
	virtual void resizeEvent(QResizeEvent*);

private slots:
	void focusChanged();
	void updateStatusMessage();
	void move(); // ADDED BY LARS PETTER MOSTAD

private:
	void generate(unsigned int seed);
	void finish();
	void renderMaze(int frame);
	void renderDone();
	void renderPause();
	void renderText(QPainter* painter, const QString& message) const;

	bool m_done;
	bool m_paused;
	bool first_mouse_click; // ADDED BY TERJE GUNDERSEN
	
	int m_total_targets;
	Maze* m_maze;
	QPoint m_start;
	QList<QPoint> m_targets;
	QLabel* m_status_message;
	QTimer* m_status_timer;
	QTimer* m_move_timer; // ADDED BY LARS PETTER MOSTAD

	bool m_show_path;
	bool m_show_time;
	bool m_show_steps;

	bool m_smooth_movement;
	int m_col_delta;
	int m_row_delta;
	QTimeLine* m_move_animation;

	Theme* m_theme;
	int m_unit;

	// Player
	QPoint m_player;
	int m_player_direction;	 // ADDED BY LARS PETTER MOSTAD
	bool m_player_firststep; // ADDED BY LARS PETTER MOSTAD
	int m_player_angle;
	int m_player_steps;
	QTime m_player_time;
	int m_player_total_time;
	unsigned int m_controls_up;
	unsigned int m_controls_down;
	unsigned int m_controls_left;
	unsigned int m_controls_right;
	unsigned int m_controls_flag;
	unsigned int m_controls_talk; // ADDED BY LARS PETTER MOSTAD
	
	Recorder recorder; // ADDED BY LARS PETTER MOSTAD
	int m_start_record_time; // ADDED BY LARS PETTER MOSTAD
};

#endif // BOARD_H
