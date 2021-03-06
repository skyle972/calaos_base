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
#include <ActionMail.h>
#include <ListeRoom.h>
#include <IPCam.h>
#include <FileDownloader.h>

using namespace Calaos;

ActionMail::ActionMail(): Action(ACTION_MAIL)
{
        sigDownload.connect(sigc::mem_fun(*this, &ActionMail::IPCDownloadDone));

        Utils::logger("rule.action.mail") << Priority::DEBUG << "ActionMail::ActionMail(): New Mail action" << log4cpp::eol;
}

ActionMail::~ActionMail()
{
        Utils::logger("rule.action.mail") << Priority::DEBUG << "ActionMail::~ActionMail(): Ok" << log4cpp::eol;
}

bool ActionMail::Execute()
{
        IPCam *camera = NULL;

        if (mail_attachment != "")
        {
                Input *in = ListeRoom::Instance().get_input(mail_attachment);
                Output *out = NULL;
                if (!in) out = ListeRoom::Instance().get_output(mail_attachment);

                if (in)
                {
                        CamInput *cin = reinterpret_cast<CamInput *>(in);
                        if (cin) camera = cin->get_cam();
                }
                if (out)
                {
                        CamOutput *cout = reinterpret_cast<CamOutput *>(out);
                        if (cout) camera = cout->get_cam();
                }
        }

        MailMessage *msg = new MailMessage();

        msg->setSender(mail_sender);
        msg->setSubject(mail_subject);

        vector<string> recips;
        split(mail_recipients, recips, ",");
        for (uint i = 0;i < recips.size();i++)
                msg->addRecipient(recips[i]);

        msg->setBodyText(mail_message);

        if (camera)
        {
                Utils::logger("rule.action.mail") << Priority::INFO << "ActionMail::Execute(): Need to download camera ("
                                << camera->get_param("name")
                                << ") attachment" << log4cpp::eol;

                string tmpFile;
                int cpt = 0;

                //Get a temporary filename
                do
                {
                        tmpFile = "/tmp/calaos_mail_attachment_";
                        tmpFile += to_string(cpt);
                        cpt++;
                }
                while (ecore_file_exists(tmpFile.c_str()));

                /* Autodestroy file downloader. Will send an ipc when done */
                FileDownloader* downloader = new FileDownloader(camera->get_picture(), tmpFile, true);
                IPC::Instance().AddHandler("downloader::" + to_string(downloader), "*",
                                           sigDownload, msg);
                downloader->Start();
        }
        else
        {
                SendMail::Instance().SendMessage(msg);

                Utils::logger("rule.action.mail") << Priority::INFO << "ActionMail::Execute(): Ok, mail is in queue" << log4cpp::eol;
        }

        return true;
}

void ActionMail::IPCDownloadDone(string source, string signal, void *listener_data, void *sender_data)
{
        if(signal == "done")
        {
                MailMessage *msg = reinterpret_cast<MailMessage *>(listener_data);
                if (msg)
                {
                        string file = *(reinterpret_cast<string *>(sender_data));
                        msg->addAttachment(file, "camera.jpg", "Camera", "image/jpeg");

                        SendMail::Instance().SendMessage(msg);
                }
        }
        else if (signal == "failed" || signal == "aborted")
        {
                MailMessage *msg = reinterpret_cast<MailMessage *>(listener_data);
                if (msg)
                {
                        SendMail::Instance().SendMessage(msg);
                }
        }
}

bool ActionMail::LoadFromXml(TiXmlElement *pnode)
{
        TiXmlElement *mail_node = pnode->FirstChildElement("calaos:mail");
        if (!mail_node) return false;

        if (mail_node->Attribute("sender")) mail_sender = mail_node->Attribute("sender");
        if (mail_node->Attribute("recipients")) mail_recipients = mail_node->Attribute("recipients");
        if (mail_node->Attribute("subject")) mail_subject = mail_node->Attribute("subject");
        if (mail_node->Attribute("attachment")) mail_attachment = mail_node->Attribute("attachment");

        //remove spaces
        replace_str(mail_recipients, " ", "");

        TiXmlText *tnode = dynamic_cast<TiXmlText *>(mail_node->FirstChild());

        if (tnode)
                mail_message = tnode->ValueStr();

        return true;
}

bool ActionMail::SaveToXml(TiXmlElement *node)
{
        TiXmlElement *action_node = new TiXmlElement("calaos:action");
        action_node->SetAttribute("type", "mail");
        node->LinkEndChild(action_node);

        TiXmlElement *mail_node = new TiXmlElement("calaos:mail");
        mail_node->SetAttribute("sender", mail_sender);
        mail_node->SetAttribute("recipients", mail_recipients);
        mail_node->SetAttribute("subject", mail_subject);
        mail_node->SetAttribute("attachment", mail_attachment);
        action_node->LinkEndChild(mail_node);

        TiXmlText *txt_node = new TiXmlText(mail_message);
        txt_node->SetCDATA(true);
        mail_node->LinkEndChild(txt_node);

        return true;
}
