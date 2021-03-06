/* Copyright 2013 - 2015 Yurii Litvinov and CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QMutex>

#include "deviceInterface.h"
#include "deviceState.h"

namespace trikKernel {
class Configurer;
}

namespace trikControl {

/// Provides direct interaction with I2C device.
/// @todo: It shall work in separate thread.
class I2cCommunicator : public DeviceInterface
{
public:
	/// Constructor.
	/// @param configurer - contains preparsed XML configuration.
	I2cCommunicator(trikKernel::Configurer const &configurer);

	~I2cCommunicator();

	/// Send data to current device, if it is connected.
	void send(QByteArray const &data);

	/// Reads data by given I2C command number and returns the result.
	int read(QByteArray const &data);

	Status status() const override;

private:
	/// Establish connection with current device.
	void connect();

	/// Disconnect from a device.
	void disconnect();

	QString const mDevicePath;
	int mDeviceId = 0;
	int mDeviceFileDescriptor;
	QMutex mLock;
	DeviceState mState;
};

}
