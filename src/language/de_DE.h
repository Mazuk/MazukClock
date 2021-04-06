#ifndef WORDCLOCK_GRID_LANG_DE_DE_H
#define WORDCLOCK_GRID_LANG_DE_DE_H

class Grid_de_DE {
	public:
	static int time_it_is[5];
	static int time_minutes[12][12];
	static int time_hours[12][6];
	static int time_minutes_alt[12][12];
	static int time_hours_alt[12][6];
	static int ding[11][12];
	static int cube[6][41];
	static int ball[7][35];

	static void setTime(int hour, int minute);
	static void setDingDong(int dingdong, int r, int g, int b);
	static void setCube(int dingdong);
	static void setBall(int dingdong);
};

typedef class Grid_de_DE Grid;

#endif
