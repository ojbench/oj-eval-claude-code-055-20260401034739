#include <iostream>
#include <algorithm>
#include "base.hpp"

using namespace std;

struct date
{
    int year, month, day;
    date() = default;
    date(int y, int m, int d) : year(y), month(m), day(d) {}

    // Stream input operator
    friend istream& operator>>(istream& is, date& d) {
        is >> d.year >> d.month >> d.day;
        return is;
    }

    // Less than operator for date comparison
    bool operator<(const date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    // Helper function to calculate days from reference point (for car_mail calculation)
    int to_days() const {
        return year * 360 + month * 30 + day;
    }
};

class mail : public object
{
protected:
    string postmark;
    date send_date;
    date arrive_date;

public:
    mail() = default;

    mail(string _contain_, string _postmark_, date send_d, date arrive_d) : object(_contain_)
    {
        postmark = _postmark_;
        send_date = send_d;
        arrive_date = arrive_d;
    }

    virtual string send_status(int y, int m, int d)
    {
        return "not send";
    }

    virtual string type()
    {
        return "no type";
    }

    virtual void print()
    {
        object::print();
        cout << "[mail] postmark: " << postmark << endl;
    }

    virtual void copy(object *o)
    {
        contain = reinterpret_cast<mail *>(o)->contain;
        postmark = reinterpret_cast<mail *>(o)->postmark;
        send_date = reinterpret_cast<mail *>(o)->send_date;
        arrive_date = reinterpret_cast<mail *>(o)->arrive_date;
    }

    virtual ~mail() {}
};

class air_mail : public mail
{
protected:
    string airlines;
    date take_off_date;
    date land_date;

public:
    air_mail() = default;

    air_mail(string _contain_, string _postmark_, date send_d, date arrive_d, date take_off, date land, string _airline) : mail(_contain_, _postmark_, send_d, arrive_d)
    {
        take_off_date = take_off;
        land_date = land;
        airlines = _airline;
    }

    string send_status(int y, int m, int d)
    {
        date ask_date(y, m, d);
        if (ask_date < send_date)
            return "mail not send";
        else if (ask_date < take_off_date)
            return "wait in airport";
        else if (ask_date < land_date)
            return "in flight";
        else if (ask_date < arrive_date)
            return "already land";
        else
            return "already arrive";
    }

    string type()
    {
        return "air";
    }

    void print()
    {
        mail::print();
        cout << "[air] airlines: " << airlines << endl;
    }

    void copy(object *o)
    {
        mail::copy(o);
        airlines = reinterpret_cast<air_mail *>(o)->airlines;
        take_off_date = reinterpret_cast<air_mail *>(o)->take_off_date;
        land_date = reinterpret_cast<air_mail *>(o)->land_date;
    }

    ~air_mail() {}
};

class train_mail : public mail
{
protected:
    string *station_name;
    date *station_time;
    int len;

public:
    train_mail() : station_name(nullptr), station_time(nullptr), len(0) {}

    train_mail(string _contain_, string _postmark_, date send_d, date arrive_d, string *sname, date *stime, int station_num) : mail(_contain_, _postmark_, send_d, arrive_d)
    {
        len = station_num;
        station_name = new string[len];
        station_time = new date[len];
        for (int i = 0; i < len; i++) {
            station_name[i] = sname[i];
            station_time[i] = stime[i];
        }
    }

    string send_status(int y, int m, int d)
    {
        date ask_date(y, m, d);
        if (ask_date < send_date)
            return "mail not send";

        // Check if arrived at destination
        if (!(ask_date < arrive_date))
            return "already arrive";

        // Check which station or in transit
        for (int i = 0; i < len; i++) {
            if (ask_date < station_time[i]) {
                // Still before this station
                if (i == 0) {
                    return "on the way to " + station_name[i];
                } else {
                    return "on the way to " + station_name[i];
                }
            } else if (!(station_time[i] < ask_date)) {
                // Equal to station time - at the station
                return "at " + station_name[i];
            }
        }

        // After all stations but before arrival
        return "on the way to destination";
    }

    string type()
    {
        return "train";
    }

    void print()
    {
        mail::print();
        cout << "[train] station_num: " << len << endl;
    }

    void copy(object *o)
    {
        mail::copy(o);
        train_mail *tm = reinterpret_cast<train_mail *>(o);

        // Delete old arrays if they exist
        if (station_name != nullptr) {
            delete[] station_name;
        }
        if (station_time != nullptr) {
            delete[] station_time;
        }

        len = tm->len;
        station_name = new string[len];
        station_time = new date[len];
        for (int i = 0; i < len; i++) {
            station_name[i] = tm->station_name[i];
            station_time[i] = tm->station_time[i];
        }
    }

    ~train_mail()
    {
        if (station_name != nullptr) {
            delete[] station_name;
        }
        if (station_time != nullptr) {
            delete[] station_time;
        }
    }
};

class car_mail : public mail
{
protected:
    int total_mile;
    string driver;

public:
    car_mail() = default;

    car_mail(string _contain_, string _postmark_, date send_d, date arrive_d, int mile, string _driver) : mail(_contain_, _postmark_, send_d, arrive_d)
    {
        total_mile = mile;
        driver = _driver;
    }

    string send_status(int y, int m, int d)
    {
        date ask_date(y, m, d);
        if (ask_date < send_date)
            return "mail not send";
        else if (!(ask_date < arrive_date)) // ask_date >= arrive_date
            return "already arrive";
        else {
            // Calculate current mile
            double used_days = (double)(ask_date.to_days() - send_date.to_days());
            double total_days = (double)(arrive_date.to_days() - send_date.to_days());
            double current_mile = (used_days / total_days) * (double)total_mile;
            return to_string(current_mile);
        }
    }

    string type()
    {
        return "car";
    }

    void print()
    {
        mail::print();
        cout << "[car] driver_name: " << driver << endl;
    }

    void copy(object *o)
    {
        mail::copy(o);
        car_mail *cm = reinterpret_cast<car_mail *>(o);
        total_mile = cm->total_mile;
        driver = cm->driver;
    }

    ~car_mail() {}
};

void obj_swap(object *&x, object *&y)
{
    object *temp = x;
    x = y;
    y = temp;
}
