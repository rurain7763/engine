#ifndef WINDOWCLOSEEVENT_H
#define WINDOWCLOSEEVENT_H

#include "EngineHeader.h"
#include "../eventbus/Event.h"

class WindowClosedEvent : public Event {
public:
    WindowClosedEvent() = default;

    std::string ToString() const {
        return "WindowClosedEvent: ";
    }
};

#endif