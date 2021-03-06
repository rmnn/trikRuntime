/* Copyright 2015 CyberTech Labs Ltd.
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

#include "mailboxFactory.h"

#include <trikKernel/configurer.h>

#include "mailbox.h"

using namespace trikNetwork;

MailboxInterface *MailboxFactory::create(int port)
{
	return new Mailbox(port);
}

MailboxInterface *MailboxFactory::create(trikKernel::Configurer const &configurer)
{
	if (configurer.isEnabled("mailbox")) {
		return new Mailbox(configurer);
	} else {
		return nullptr;
	}
}
