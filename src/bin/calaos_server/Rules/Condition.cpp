/******************************************************************************
**  Copyright (c) 2007-2008, Calaos. All Rights Reserved.
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
**  along with Foobar; if not, write to the Free Software
**  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
******************************************************************************/
#include <Condition.h>

using namespace Calaos;

Condition::Condition(int type): condition_type(type)
{
        Utils::logger("rule.condition") << Priority::DEBUG << "Condition::Condition(): New condition" << log4cpp::eol;
}

Condition::~Condition()
{
        Utils::logger("rule.condition") << Priority::DEBUG << "Condition::~Condition(): Ok" << log4cpp::eol;
}

bool Condition::Evaluate()
{

        Utils::logger("rule.condition") << Priority::ERROR << "Condition::Evaluate(): Can't evaluate base Condition class !" << log4cpp::eol;

        return false;
}
