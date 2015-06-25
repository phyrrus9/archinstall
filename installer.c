#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <stdarg.h>

typedef struct window_container
{
	WINDOW *con;
	WINDOW *win;
	int lines, cols;
	int xp, yp;
} WCON;

WCON *newcon(int lines, int cols, int y, int x)
{
	WCON *ret = NULL;
	ret = malloc(sizeof(WCON));
	ret->lines = lines;
	ret->cols = cols;
	ret->xp = x;
	ret->yp = y;
	ret->con = newwin(lines+2, cols+2, y, x);
	ret->win = newwin(lines, cols, y+1,x+1);
	wclear(ret->con);
	wclear(ret->win);
	return ret;
}

void resizecon(WCON *c, int cols, int lines)
{
	c->cols = cols;
	c->lines = lines;
	wresize(c->con, lines+2, cols+2);
	wresize(c->win, lines, cols);
}

void movecon(WCON *c, int y, int x)
{
	c->xp = x;
	c->yp = y;
	mvwin(c->con, y, x);
	mvwin(c->win, y+1, x+1);
}

void titlecon(WCON *c, const char *t)
{
	int len = 0;
	if (t) len = strlen(t) + 2;
	wborder(c->con, 0, 0, 0, 0, 0, 0, 0, 0);
	if (len > 0)
	{
		wmove(c->con, 0,
		      (c->cols - len) / 2 + 1);
		wprintw(c->con, "|%s|", t);
	}
	wrefresh(c->con);
}

void bordercon(WCON *c)
{ titlecon(c, NULL); }

void printcon(WCON *c, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vwprintw(c->win, fmt, args);
	va_end(args);
}

void refreshcon(WCON *c)
{
	wrefresh(c->con);
	wrefresh(c->win);
}

WINDOW *conwin(WCON *c)
{ return c->win; }

void deletecon(WCON *c)
{
	wclear(c->con);
	wclear(c->win);
	wrefresh(c->con);
	wrefresh(c->win);
	delwin(c->con);
	delwin(c->win);
	free(c);
}

int menu(const char *title, int size, int selected, ...)
/*args:
  name (const char *)
  return:
  ID of selected
*/
{
	char ** names = malloc(sizeof(char *) * size);
	int i, longeststr = strlen(title) + 6, tmp;
	int inchar = 0;
	va_list args;
	WCON *wc;
	WINDOW *win;
	initscr();
	noecho();
	refresh();
	keypad(stdscr, TRUE);
	curs_set(0);
	va_start(args, selected);
	for (i = 0; i < size; i++)
	{
		names[i] = va_arg(args, char*);
		tmp = strlen(names[i]);
		if (tmp > longeststr) longeststr = tmp + 4;
	}
	wc = newcon(size, longeststr, (LINES/2)-size, (COLS/2)-(longeststr/2));
	win = conwin(wc);
	titlecon(wc, title);
	while (inchar != '\n')
	{
		wclear(win);
		for (i = 0; i < size; i++)
		{
			wattroff(win, A_STANDOUT);
			if (i == selected)
				wattron(win, A_STANDOUT);
			wprintw(win, "%s\n", names[i]);
		}
		wrefresh(win);
		inchar = getch();
		switch (inchar)
		{
			case KEY_UP:
				if (selected > 0) --selected;
				break;
			case KEY_DOWN:
				if (selected < size - 1) ++selected;
				break;
			default: break;
		}
	}
	va_end(args);
	free(names);
	deletecon(wc);
	endwin();
	return selected;
}

int pre_install()
{
	int opt = 0;
	do
	{
		opt = menu("Pre Installation", 5, opt,
			"Partition Disks",
			"Format rootfs",
			"Mount rootfs",
			"Connect to the internet",
			"Done");
		switch (opt)
		{
			case 0: //partition
				break;
			case 1: //format
				break;
			case 2: //mount
				break;
			case 3: //connect
				break;
			default: break;
		}
	} while (opt != 4);
	return 0;
}

int core_install()
{
	int opt = 0;
	do
	{
		opt = menu("Core Installation", 7, opt,
			"Select targets",
			"Bootstrap",
			"Generate fstab",
			"Set hostname",
			"Set timezone",
			"Make ramdisk",
			"Done");
		switch (opt)
		{
			case 0: //set targets
				break;
			case 1: //bootstrap
				break;
			case 2: //genfstab
				break;
			case 3: //set hostname
				break;
			case 4: //set timezone
				break;
			case 5: //make ramdisk
				break;
			default: break;
		}
	} while (opt != 6);
	return 0;
}

int post_install()
{
	int opt = 0;
	do
	{
		opt = menu("Post Installation", 5, opt,
			"Set root password",
			"Create user account",
			"Install GRUB",
			"Finalize",
			"Done");
		switch (opt)
		{
			case 0: //passwd root
				break;
			case 1: //create user acct
				break;
			case 2: //install grub
				break;
			case 3: //sync and umount
				break;
			default: break;
		}
	} while (opt != 4);
	return 0;
}

void main_menu()
{
	int opt = 0;
	do
	{
		opt = menu("ArchLinux Installer", 5, opt,
			"Pre Install",
			"Core Install",
			"Post Install",
			"Reboot",
			"Exit");
		switch (opt)
		{
			case 0: //pre install
				pre_install();
				break;
			case 1: //core install
				core_install();
				break;
			case 2: //post install
				post_install();
				break;
			case 3: //reboot
////////////////////////////////system("reboot");
				break;
			default: break;
		}
	} while (opt != 4);
}

int main()
{
	main_menu();
	return 0;
}
