/****************************************************************************
 *  Copyright (C) 2019 RoboMaster.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "handle.h"

namespace roborts_sdk {
Handle::Handle(std::string serial_port) {
  serial_port_ = serial_port;
  device_ = std::make_shared<SerialDevice>(serial_port_, 115200);
  protocol_ = std::make_shared<Protocol>(device_);

}
bool Handle::Init(){
  if (!device_->Init()) {
    LOG_ERROR<<"Can not open device: " <<serial_port_
    <<". Please check if the USB device is inserted and connection is configured correctly!";
    return false;
  }
  LOG_INFO<<"Connection to "<<serial_port_;
  if (!protocol_->Init()) {
    LOG_ERROR<<"Protocol initialization failed.";
    return false;
  }
  executor_ = std::make_shared<Executor>(shared_from_this());
  LOG_INFO<<"Initialization of protocol layer and dispatch layer succeeded. ";
  return true;
}
std::shared_ptr<Protocol>& Handle::GetProtocol() {
  return protocol_;
}

void Handle::Spin() {

    // 处理底盘发给pc的信息
    int count = 0;
  for (auto sub :subscription_factory_) {
   // LOG_INFO<<"subscription_factory_ Spin  "<< 111   ;
    executor_->ExecuteSubscription(sub);
      count++;
  }

  count = 0;
  // 处理 ?? de xiaoxi
  for (auto client :client_factory_) {
    //LOG_INFO<<"client_factory_ Spin ";
    executor_->ExecuteClient(client);
    count++;
  }
    LOG_INFO<<"client_factory_ Spin count "<< count   ;
    count = 0;
  for (auto service :service_factory_) {
    //LOG_INFO<<"subscription_factory_ Spin ";
    executor_->ExecuteService(service);
    count++;
  }

    LOG_INFO<<"service_factory_ Spin count "<< count   ;
}
}