/******************************************************************************
**  Copyright (c) 2006-2011, Calaos. All Rights Reserved.
**
**  This file is part of Calaos Home.
**
**  Calaos Home is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 3 of the License, or
**  (at your option) any later version.
**
**  Calaos Home is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Calaos; if not, write to the Free Software
**  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
******************************************************************************/
#ifndef ActivityScheduleScenarioView_H
#define ActivityScheduleScenarioView_H

#include <Utils.h>

#include "ActivityView.h"
#include "CalaosModel.h"
#include "GenlistItemSimple.h"

using namespace Utils;

class ActivityScheduleScenarioView: public ActivityView
{
        private:
                TimeRangeInfos range_infos;

                Evas_Object *schedule_list, *month_list;

                GenlistItemSimple *item_all;
                vector<GenlistItemSimple *> items_months;
                vector<GenlistItemSimple *> items_periods;

                Evas_Object *popup;
                Evas_Object *pager_popup;

                Evas_Object *spin_start_hours;
                Evas_Object *spin_start_min;
                Evas_Object *spin_start_sec;
                Evas_Object *spin_start_ms;

                Evas_Object *spin_end_hours;
                Evas_Object *spin_end_min;
                Evas_Object *spin_end_sec;
                Evas_Object *spin_end_ms;

                vector<GenlistItemSimple *> week_days;

                bool cycle;

                void buttonPressed(void *data, Evas_Object *_edje, std::string emission, std::string source);

                void itemAllYearSelected(void *data);
                void itemMonthSelected(void *data, GenlistItemSimple *item);
                void itemPeriodSelected(void *data, GenlistItemSimple *item);

                void reloadTimeRanges();

                void buttonValidEndClick(void *data, Evas_Object *edje_object, string emission, string source);
                void buttonValidWeekClick(void *data, Evas_Object *edje_object, string emission, string source);
                void buttonBackClick(void *data, Evas_Object *edje_object, string emission, string source);

                void unselectWeekDays(void *data);
                void unselectAllWeekDays(void *data);

                void headerWeekButtonClick(string bt);

        public:
                ActivityScheduleScenarioView(Evas *evas, Evas_Object *parent);
                ~ActivityScheduleScenarioView();

                virtual void resetView();

                void setTimeRangeInfos(TimeRangeInfos &tr, bool _cycle) { range_infos = tr; reloadTimeRanges(); cycle = _cycle; }

                sigc::signal<void, TimeRangeInfos &> buttonValidPressed;
};

#endif // ActivityScheduleScenarioView_H
