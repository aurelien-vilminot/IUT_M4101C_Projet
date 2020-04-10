#include <unistd.h>             // sleep()
#include <chrono>

#include "SleepChronogram.h"

using namespace std;

string nsUtil::clrscr ("\033[2J");    //    Clear screen

#define GOTOXY nsUtil::gotoxy

GOTOXY::gotoxy (int x, int y) : m_x (x), m_y (y) {}

ostream & nsUtil::operator << (ostream & os, const gotoxy & m)
{
    return os << "\033[" << m.m_y << ';' << m.m_x << 'H';
}

#undef GOTOXY

void nsUtil::SleepChronogram (unsigned Num,
                              unsigned & Col,
                              char Status,
                              unsigned Duree,
                              mutex & ioMutex,
                              unsigned Decal   /* = 10u */,
                              unsigned Periode /* = 1u  */)

{
    for (unsigned i = Duree; i--; )
    {
        {
            lock_guard <mutex> Lock (ioMutex);
            cout << gotoxy (Col++, Num + Decal) << Status << flush;
        }
        this_thread::sleep_for (chrono::seconds (Periode));
    }

} // SleepChronogr()
