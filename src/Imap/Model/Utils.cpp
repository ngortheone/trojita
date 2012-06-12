/* Copyright (C) 2006 - 2011 Jan Kundrát <jkt@gentoo.org>

   This file is part of the Trojita Qt IMAP e-mail client,
   http://trojita.flaska.net/

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or the version 3 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "Utils.h"
#include <cmath>
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QProcess>
#include <QSslError>
#include <QSysInfo>
#include <QTextDocument>

#ifdef TROJITA_MOBILITY_SYSTEMINFO
#include <QSystemDeviceInfo>
#endif

namespace Imap
{
namespace Mailbox
{

QString PrettySize::prettySize(uint bytes, const ShowBytesSuffix compactUnitFormat)
{
    if (bytes == 0) {
        return tr("0");
    }
    int order = std::log(static_cast<double>(bytes)) / std::log(1024.0);
    QString suffix;
    if (order <= 0) {
        if (compactUnitFormat == COMPACT_FORM)
            return QString::number(bytes);
        else
            return tr("%1 bytes").arg(QString::number(bytes));
    } else if (order == 1) {
        suffix = tr("kB");
    } else if (order == 2) {
        suffix = tr("MB");
    } else if (order == 3) {
        suffix = tr("GB");
    } else {
        // make sure not to show wrong size for those that have > 1024 TB e-mail messages
        order = 4;
        suffix = tr("TB"); // shame on you for such mails
    }
    return tr("%1 %2").arg(QString::number(bytes / std::pow(1024.0, order), 'f', 1), suffix);
}

QString persistentLogFileName()
{
    QString logFileName = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    if (logFileName.isEmpty()) {
        logFileName = QDir::homePath() + QLatin1String("/.trojita-connection-log");
    } else {
        QDir().mkpath(logFileName);
        logFileName += QString::fromAscii("/trojita-connection-log");
    }
    return logFileName;
}

QString systemPlatformVersion()
{
    QString os = QString::fromAscii(""
#ifdef Q_OS_AIX
                                    "AIX"
#endif
#ifdef Q_OS_BSD4
#ifndef Q_OS_MAC
                                    "AnyBSD4.4"
#endif
#endif
#ifdef Q_OS_BSDI
                                    "BSD/OS"
#endif
#ifdef Q_OS_CYGWIN
                                    "Cygwin"
#endif
#ifdef Q_OS_DGUX
                                    "DG/UX"
#endif
#ifdef Q_OS_DYNIX
                                    "DYNIX/ptx"
#endif
#ifdef Q_OS_FREEBSD
                                    "FreeBSD"
#endif
#ifdef Q_OS_HPUX
                                    "HP-UX"
#endif
#ifdef Q_OS_HURD
                                    "Hurd"
#endif
#ifdef Q_OS_IRIX
                                    "Irix"
#endif
#ifdef Q_OS_LINUX
                                    "Linux"
#endif
#ifdef Q_OS_LYNX
                                    "LynxOS"
#endif
#ifdef Q_OS_MAC
                                    "MacOS"
#endif
#ifdef Q_OS_MSDOS
                                    "MSDOS"
#endif
#ifdef Q_OS_NETBSD
                                    "NetBSD"
#endif
#ifdef Q_OS_OS2
                                    "OS/2"
#endif
#ifdef Q_OS_OPENBSD
                                    "OpenBSD"
#endif
#ifdef Q_OS_OS2EMX
                                    "OS2EMX"
#endif
#ifdef Q_OS_OSF
                                    "HPTru64UNIX"
#endif
#ifdef Q_OS_QNX
                                    "QNXNeutrino"
#endif
#ifdef Q_OS_RELIANT
                                    "ReliantUNIX"
#endif
#ifdef Q_OS_SCO
                                    "SCOOpenServer5"
#endif
#ifdef Q_OS_SOLARIS
                                    "Solaris"
#endif
#ifdef Q_OS_SYMBIAN
                                    "Symbian"
#endif
#ifdef Q_OS_ULTRIX
                                    "Ultrix"
#endif
#ifdef Q_OS_UNIXWARE
                                    "Unixware"
#endif
#ifdef Q_OS_WIN32
                                    "Windows"
#endif
#ifdef Q_OS_WINCE
                                    "WinCE"
#endif
                                   );
#ifdef Q_OS_UNIX
    if (os.isEmpty()) {
        os = "Unix";
    }
#endif

    QString ws = ""
#ifdef Q_WS_X11
                 "X11"
#endif
#ifdef Q_WS_S60
                 "S60"
#endif
#ifdef Q_WS_MAC
                 "Mac"
#endif
#ifdef Q_WS_QWS
                 "QWS"
#endif
#ifdef Q_WS_WIN
                 "Win"
#endif
                 ;

    static QString platformVersion;
#ifdef TROJITA_MOBILITY_SYSTEMINFO
    if (platformVersion.isEmpty()) {
        QtMobility::QSystemDeviceInfo device;
        if (device.productName().isEmpty()) {
            platformVersion = device.manufacturer() + QLatin1String(" ") + device.model();
        } else {
            platformVersion = QString::fromAscii("%1 %2 (%3)").arg(device.manufacturer(), device.productName(), device.model());
        }
    }
#endif
    if (platformVersion.isEmpty()) {
#ifdef Q_OS_WIN32
        switch (QSysInfo::WindowsVersion) {
        case QSysInfo::WV_32s:
            platformVersion = "3.1";
            break;
        case QSysInfo::WV_95:
            platformVersion = "95";
            break;
        case QSysInfo::WV_98:
            platformVersion = "98";
            break;
        case QSysInfo::WV_Me:
            platformVersion = "Me";
            break;
        case QSysInfo::WV_NT:
            platformVersion = "NT";
            break;
        case QSysInfo::WV_2000:
            platformVersion = "2000";
            break;
        case QSysInfo::WV_XP:
            platformVersion = "XP";
            break;
        case QSysInfo::WV_2003:
            platformVersion = "2003";
            break;
        case QSysInfo::WV_VISTA:
            platformVersion = "Vista";
            break;
        case QSysInfo::WV_WINDOWS7:
            platformVersion = "7";
            break;
        }
#endif
#ifdef Q_OS_WINCE
        switch (QSysInfo::WindowsVersion) {
        case QSysInfo::WV_CE:
            platformVersion = "CE";
            break;
        case QSysInfo::WV_CENET:
            platformVersion = "CE.NET";
            break;
        case QSysInfo::WV_CE_5:
            platformVersion = "CE5.x";
            break;
        case QSysInfo::WV_CE_6:
            platformVersion = "CE6.x";
            break;
        }
#endif
#ifdef Q_WS_S60
switch (QSysInfo:s60Version()) {
        case QSysInfo::SV_S60_3_1:
            platformVersion = "S60r3fp1";
            break;
        case QSysInfo::SV_S60_3_2:
            platformVersion = "S60r3fp2";
            break;
        case QSysInfo::SV_S60_5_0:
            platformVersion = "S60r5";
            break;
        case QSysInfo::SV_S60_5_1:
            platformVersion = "S60r5fp1";
            break;
        case QSysInfo::SV_S60_5_2:
            platformVersion = "S60r5fp2";
            break;
        case QSysInfo::SV_S60_Unnown:
            platformVersion = "SV_Unknown";
            break;
        }
#endif
#ifdef Q_OS_SYMBIAN
        switch (QSysInfo::symbianVersion()) {
        case QSysInfo::SV_SF_1:
            platformVersion = "Symbian^1";
            break;
        case QSysInfo::SV_SF_2:
            platformVersion = "Symbian^2";
            break;
        case QSysInfo::SV_SF_3:
            platformVersion = "Symbian^3";
            break;
        case QSysInfo::SV_SF_4:
            platformVersion = "Symbian^4";
            break;
        }
#endif
#ifdef Q_OS_MAC
        switch (QSysInfo::MacintoshVersion) {
        case QSysInfo::MV_9:
            platformVersion = "9.0";
            break;
        case QSysInfo::MV_10_0:
            platformVersion = "X 10.0";
            break;
        case QSysInfo::MV_10_1:
            platformVersion = "X 10.1";
            break;
        case QSysInfo::MV_10_2:
            platformVersion = "X 10.2";
            break;
        case QSysInfo::MV_10_3:
            platformVersion = "X 10.3";
            break;
        case QSysInfo::MV_10_4:
            platformVersion = "X 10.4";
            break;
        case QSysInfo::MV_10_5:
            platformVersion = "X 10.5";
            break;
        case QSysInfo::MV_10_6:
            platformVersion = "X 10.6";
            break;
        }
#endif
        if (platformVersion.isEmpty()) {
            // try to call the lsb_release
            QProcess *proc = new QProcess(0);
            proc->start("lsb_release", QStringList() << QLatin1String("-s") << QLatin1String("-d"));
            proc->waitForFinished();
            platformVersion = QString::fromLocal8Bit(proc->readAll()).trimmed().replace(QLatin1String("\""), QString()).replace(QLatin1String(";"), QLatin1String(","));
            proc->deleteLater();
        }
    }
    return QString::fromAscii("Qt/%1; %2; %3; %4").arg(qVersion(), ws, os, platformVersion);
}

/** @short Produce a properly formatted HTML string which won't overflow the right edge of the display */
QByteArray CertificateUtils::htmlHexifyByteArray(const QByteArray &rawInput)
{
    QByteArray inHex = rawInput.toHex();
    QByteArray res;
    const int stepping = 4;
    for (int i = 0; i < inHex.length(); i += stepping) {
        // The individual blocks are formatted separately to allow line breaks to happen
        res.append("<code style=\"font-family: monospace;\">");
        res.append(inHex.mid(i, stepping));
        if (i + stepping < inHex.size()) {
            res.append(":");
        }
        // Produce the smallest possible space. "display: none" won't notice the space at all, leading to overly long lines
        res.append("</code><span style=\"font-size: 1px\"> </span>");
    }
    return res;
}

QString CertificateUtils::chainToHtml(const QList<QSslCertificate> &sslChain)
{
    QStringList certificateStrings;
    Q_FOREACH(const QSslCertificate &cert, sslChain) {
        certificateStrings << tr("<li><b>CN</b>: %1,<br/>\n<b>Organization</b>: %2,<br/>\n"
                                 "<b>Serial</b>: %3,<br/>\n"
                                 "<b>SHA1</b>: %4,<br/>\n<b>MD5</b>: %5</li>").arg(
                                  Qt::escape(cert.subjectInfo(QSslCertificate::CommonName)),
                                  Qt::escape(cert.subjectInfo(QSslCertificate::Organization)),
                                  cert.serialNumber(),
                                  htmlHexifyByteArray(cert.digest(QCryptographicHash::Sha1)),
                                  htmlHexifyByteArray(cert.digest(QCryptographicHash::Md5)));
    }
    return sslChain.isEmpty() ?
                tr("<p>The remote side doesn't have a certificate.</p>\n") :
                tr("<p>This is the certificate chain of the connection:</p>\n<ul>%1</ul>\n").arg(certificateStrings.join(tr("\n")));
}

QString CertificateUtils::errorsToHtml(const QList<QSslError> &sslErrors)
{
    QStringList sslErrorStrings;
    Q_FOREACH(const QSslError &e, sslErrors) {
        sslErrorStrings << tr("<li>%1</li>").arg(Qt::escape(e.errorString()));
    }
    return sslErrors.isEmpty() ?
                QString("<p>According to your system's policy, this connection is secure.</p>\n") :
                tr("<p>The connection triggered the following SSL errors:</p>\n<ul>%1</ul>\n").arg(sslErrorStrings.join(tr("\n")));
}

void CertificateUtils::formatSslState(const QList<QSslCertificate> &sslChain, const QList<QSslCertificate> &oldSslChain,
                                      const QByteArray &oldCertificatePem, const QList<QSslError> &sslErrors,
                                      QString *title, QString *message, IconType *icon)
{
    bool certificateHasChanged = sslChain != oldSslChain && ! oldCertificatePem.isEmpty();
    bool wasAboutToExpire = sslChain.first().expiryDate() < QDateTime::currentDateTime().addDays(14);
    bool sameTrustChain = sslChain.size() > 1 && sslChain.size() == oldSslChain.size() &&
            std::equal(sslChain.constBegin() + 1, sslChain.constEnd(), oldSslChain.constBegin() + 1);

    if (certificateHasChanged) {
        if (sslErrors.isEmpty()) {
            if (sameTrustChain) {
                // The only difference is in the first certificate of the chain
                if (wasAboutToExpire) {
                    // It was going to expire in two weeks; it's probably fair to assume that this is a regular replacement
                    *icon = Information;
                    *title = tr("Renewed SSL certificate");
                    *message = tr("<p>The IMAP server got a new SSL certificate from the same Certificate Authority; "
                                  "the old one was about to expire soon.  It is probably safe to trust this certificate."
                                  "</p>\n%1\n<p>Would you like to proceed and remember the new certificate?</p>").
                            arg(chainToHtml(sslChain));
                } else {
                    // The old one was not about to expire; that doesn't mean that there's a problem, though
                    *icon = Question;
                    *title = tr("Renewed SSL certificate");
                    *message = tr("<p>The IMAP server got a new SSL certificate from the same Certificate Authority (CA), "
                                  "even though the old one was still valid.</p>\n"
                                  "%1\n<p>Would you like to proceed and remember the new certificate?</p>").
                            arg(chainToHtml(sslChain));
                }
            } else {
                // Another certificate with completely different CA, but trusted anyway
                *icon = Warning;
                *title = tr("Different SSL certificate");
                *message = tr("<p>The SSL certificate has changed and is issued by another Certificate Authority (CA). "
                              "Your system configuration is set to accept such certificates anyway.</p>\n%1\n"
                              "<p>Would you like to connect and remember the new certificate?</p>")
                        .arg(chainToHtml(sslChain));
            }
        } else {
            // changed certificate which is not trusted per systemwide policy
            if (sameTrustChain) {
                if (wasAboutToExpire) {
                    *icon = Information;
                    *title = tr("Renewed SSL certificate");
                    *message = tr("<p>The IMAP server got a new SSL certificate from the same Certificate Authority; "
                                  "the old one was about to expire soon.</p>\n%1\n%2\n"
                                  "<p>Would you like to proceed and remember the new certificate?</p>").
                            arg(chainToHtml(sslChain), errorsToHtml(sslErrors));
                } else {
                    *icon = Question;
                    *title = tr("Renewed SSL certificate");
                    *message = tr("<p>The IMAP server got a new SSL certificate from the same Certificate Authority (CA), "
                                  "even though the old one was still valid.</p>\n%1\n%2\n"
                                  "<p>Would you like to proceed and remember the new certificate?</p>").
                            arg(chainToHtml(sslChain), errorsToHtml(sslErrors));
                }
            } else {
                *title = tr("SSL looks fishy");
                *message = tr("<p>The SSL certificate of the IMAP server has changed since the last time and your system doesn't "
                              "believe that the new certificate is genuine.</p>\n%1\n%2\n"
                              "<p>Would you like to connect anyway and remember the new certificate?</p>").
                        arg(chainToHtml(sslChain), errorsToHtml(sslErrors));
                *icon = Critical;
            }
        }
    } else {
        if (sslErrors.isEmpty()) {
            // this is the first time and the certificate looks valid -> accept
            *title = tr("Accept SSL connection?");
            *message = tr("<p>This is the first time you're connecting to this IMAP server; the certificate is trusted "
                          "by this system.</p>\n%1\n%2\n"
                          "<p>Would you like to connect and remember this certificate for the next time?</p>")
                    .arg(chainToHtml(sslChain), errorsToHtml(sslErrors));
            *icon = Information;
        } else {
            *title = tr("Accept SSL connection?");
            *message = tr("<p>This is the first time you're connecting to this IMAP server and the server certificate failed "
                          "validation test.</p>\n%1\n\n%2\n"
                          "<p>Would you like to connect and remember this certificate for the next time?</p>")
                    .arg(chainToHtml(sslChain), errorsToHtml(sslErrors));
            *icon = Question;
        }
    }
}

}

/** @short Return current date in the RFC2822 format

This function will return RFC2822-formatted current date with proper timezone information and all the glory details.
It's rather surprising how hard is to do that in Qt -- why is there no accessor for the timezone info which is *already stored*
in QDateTime?
*/
QString currentDateToRfc2822()
{
    // At first, try to find out the offset of the current timezone. That's the part which sucks.
    // If there's a more Qt-ish way of doing that, please let me know.
    QDateTime now = QDateTime::currentDateTime();
    // that's right, both of these command are actually needed
    QDateTime nowUtc = now.toUTC();
    nowUtc.setTimeSpec(Qt::LocalTime);

    // Got to cast to a signed type to prevent unsigned underflow here. Also go to 64bits because otherwise there'd
    // a problem when the value is out-of-range for an int32.
    int minutesDifference = (static_cast<qint64>(now.toTime_t()) - static_cast<qint64>(nowUtc.toTime_t())) / 60;
    int tzOffsetHours = qAbs(minutesDifference) / 60;
    int tzOffsetMinutes = qAbs(minutesDifference) % 60;
    // The rest is just a piece of cake now
    return QDateTime::currentDateTime().toString(QLatin1String("ddd, dd MMM yyyy hh:mm:ss ")) +
            QLatin1String(minutesDifference >= 0 ? "+" : "-") +
            QString::number(tzOffsetHours).rightJustified(2, QLatin1Char('0')) +
            QString::number(tzOffsetMinutes).rightJustified(2, QLatin1Char('0'));
}

}
