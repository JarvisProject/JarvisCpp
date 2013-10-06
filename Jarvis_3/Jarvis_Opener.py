import mutex
from multiprocessing import Process, Pipe, Pool
from subprocess import Popen, PIPE
from googlevoice.util import input
from googlevoice import Voice
import win32api, win32con, win32gui
import subprocess
import threading
import sys
import time
import speech
import getpass
import re
import imaplib
import email
import urllib2

# Voice global object
voice = Voice()
voice.login('jarvisatyourservice1@gmail.com', '15963212')

# Mail global object
mail = imaplib.IMAP4_SSL('imap.gmail.com', '993')
mail.login( 'jarvisatyourservice1@gmail.com', '15963212')

def internet_on():
    try:
        response=urllib2.urlopen('http://google.com/',timeout=1)
        return True
    except urllib2.URLError as err: pass
    return False

def click(x,y):
    win32api.SetCursorPos((x,y))
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTDOWN,x,y,0,0)
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP,x,y,0,0)

###############################################
# NEW IMPLEMENTATION
# ==> mutex based function threads 
###############################################
mtx = mutex.mutex()

def write_to_jarvis(args):
    jarvis_out, line = args
    jarvis_out.write(line)

## Console handler
def console_thread(jarvis_pipe):
    jarvis_in = jarvis_pipe.stdin
    jarvis_out = jarvis_pipe.stdout
    while True:
        line = sys.stdin.readline()
        # data is ready.. 
        # lock the mutex and communicate with jarvis
        mtx.lock(write_to_jarvis, (jarvis_in, line))
        print "Console::Sending to Jarvis.exe: ", line
        mtx.unlock()

## SMS handler
def delete_inbox():
    mail.select(mailbox = 'inbox', readonly = False)
    resp, items = mail.search(None, 'All')
    email_ids  = items[0].split()
    mail.store(email_ids[-1], '+FLAGS', '\\Deleted')
    mail.expunge()

def sendsms(line):
    voice.send_sms("8589976724", line)

def sms_readline():
    while True:
        mail.select()
        #searches how many unseen "['OK']['<number>']"
        aaa, bbb = mail.search(None,'UNSEEN')
        #if the number of unseen is more than 0, read it
        msg_recv = ""
        if bbb != ['']:
            #set to inbox
            mail.select('inbox')

            #get uids of all messages
            result, data = mail.uid('search', None, 'ALL') 
            uids = data[0].split()

            #read the lastest message
            result, data = mail.uid('fetch', uids[-1], '(RFC822)')
            m = email.message_from_string(data[0][1])
            line = m.get_payload()
            delete_inbox()
            return line

        time.sleep(0.5)

def sms_thread(jarvis_pipe):
    jarvis_in = jarvis_pipe.stdin
    jarvis_out = jarvis_pipe.stdout
    while True:
        line = sms_readline()
        # data is ready.. 
        # lock the mutex and communicate with jarvis
        line_jarvis = ("SMS--*--$%s" % line)
        mtx.lock(write_to_jarvis, (jarvis_in, line_jarvis))
        # read response from jarvis
        from_jarvis = jarvis_out.readline()
        # send it back to SMS
        sendsms(from_jarvis)
        mtx.unlock()

## Voice handler
def voice_thread(jarvis_pipe):
    jarvis_in = jarvis_pipe.stdin
    jarvis_out = jarvis_pipe.stdout
    while True:
        line = speech.input()
        # data is ready.. 
        # lock the mutex and communicate with jarvis
        mtx.lock(write_to_jarvis, (jarvis_in, line+'\n'))
        mtx.unlock()


### Jarvis engine
def CallJarvis():
    # p = Popen("C:/James/Visual Studio/C++ Program Files/Jarvis_3/Debug/Jarvis_3.exe", stdin=PIPE, stdout=PIPE, shell=True) #stderr=child_conn)
    p = Popen("E:/Jason/Projects/JarvisCpp/Debug/Jarvis_3.exe", stdin=PIPE, stdout=PIPE, shell=True) #stderr=child_conn)
    return p
    
if __name__ == '__main__':
    # Launch Jarvis executable
    pJarvis = CallJarvis()

    # Console thread
    console_thr = threading.Thread(target=console_thread, args=(pJarvis,) )
    console_thr.start()
 
    # SMS thread
    sms_thr = threading.Thread(target=sms_thread, args=(pJarvis,) )
    sms_thr.start()
 
    # Voice thread
    voice_thr = threading.Thread(target=voice_thread, args=(pJarvis,) )
    voice_thr.start()

    # Wait for all the threads to finish
    console_thr.join()
    sms_thr.join()
    voice_thr.join()

    exit()
