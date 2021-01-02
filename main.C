#include <cstdio>
#include <fstream>
#include <chrono>
#include <unistd.h>

#include <X11/Xlib.h>

int main() {

  std::string battery_level;
  std::string battery_status;
  std::ifstream bat_level_f;
  std::ifstream bat_status_f;

  while (1) {
    // Open X stuff
    auto dpy = XOpenDisplay(NULL);
    auto screen = DefaultScreen(dpy);
    auto root = RootWindow(dpy, screen);

    // Get battery level
    bat_level_f.open("/sys/class/power_supply/BAT0/capacity");
    bat_level_f >> battery_level;
    bat_level_f.close();
    bat_status_f.open("/sys/class/power_supply/BAT0/status");
    bat_status_f >> battery_status;
    bat_status_f.close();

    // Get time
    auto now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&tt);

    char result[80];
    sprintf(result, "%02u:%02u  %s%%  %s", local_tm.tm_hour, local_tm.tm_min,
        battery_level.c_str(), battery_status.c_str());

    XStoreName(dpy, root, result);
    XCloseDisplay(dpy);
    sleep(5);
  }
}
