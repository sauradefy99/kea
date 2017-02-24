// Copyright (C) 2016-2017 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <agent/ctrl_agent_controller.h>
#include <agent/ctrl_agent_process.h>

using namespace isc::process;

namespace isc {
namespace agent {

/// @brief Defines the application name, this is passed into base class
/// it may be used to locate configuration data and appears in log statement.
const char* CtrlAgentController::agent_app_name_ = "Control-agent";

/// @brief Defines the executable name. This is passed into the base class
const char* CtrlAgentController::agent_bin_name_ = "kea-ctrl-agent";

DControllerBasePtr&
CtrlAgentController::instance() {
    // If the instance hasn't been created yet, create it.  Note this method
    // must use the base class singleton instance methods.
    if (!getController()) {
        DControllerBasePtr controller_ptr(new CtrlAgentController());
        setController(controller_ptr);
    }

    return (getController());
}

DProcessBase*
CtrlAgentController::createProcess() {
    // Instantiate and return an instance of the D2 application process. Note
    // that the process is passed the controller's io_service.
    return (new CtrlAgentProcess(getAppName().c_str(), getIOService()));
}

CtrlAgentController::CtrlAgentController()
    : DControllerBase(agent_app_name_, agent_bin_name_) {
}

CtrlAgentController::~CtrlAgentController() {
}

} // namespace isc::agent
} // namespace isc
