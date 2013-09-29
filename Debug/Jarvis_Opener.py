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

class ConditionValue:
    def __init__(self, n):
        self.val = n
        self.cv = threading.Condition()
        
    def wait_for(self,n,wait_time=None):
        self.cv.acquire()
        while self.val != n:
            self.cv.wait(wait_time)
            
    def release():
        self.cv.release()
        
    def set_release(self,n):
        self.val = n
        self.cv.notifyAll()
        self.cv.release()
        
    def get(self):
        return self.val
    
cval = ConditionValue(0)
speech_parent, speech_child = Pipe()

### Voice input        
def speech_callback(phrase, listener):
    #prints what I said
    speech_child.send(": %s\n" %(phrase))
##    sys.stdout.flush()
    if phrase == "turn off speech" or phrase == "Turn off speech" or \
       phrase == "turnoff speech" or phrase == "Turnoff speech":
        #if I say "turn off speech"
        #Jarvis says "yes sir"
        speech.say("Speech uninitialized sir.")
        listener.stoplistening()
        speech_do_work(False)
    if phrase == "turn on speech" or phrase == "Turn on speech":
        #if I say "turn on speech"
        #Jarvis says "Speech initialized sir"
        speech.say("Speech initialized sir")
        listener.stoplistening()
        speech_do_work(True)  
def speech_do_work(boolean):
    
    if boolean:
        #if I say "turn on speech"
        #Jarvis will listen to anything I say and print it
        speech.listenforanything(speech_callback)
    else:
        #if I say "turn off speech"
        #it will wait for me to say "turn on speech"
        speech.listenfor(["turn on speech"], speech_callback)
        
### SMS Input    
#log in
voice = Voice()
mail = imaplib.IMAP4_SSL('imap.gmail.com', '993')

voice.login("jarvisatyourservice1@gmail.com", "15963212")
mail.login('jarvisatyourservice1@gmail.com', '15963212')


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
def SMS(child_conn):
    while True:
        cval.wait_for(0)
##        mail = imaplib.IMAP4_SSL('imap.gmail.com','993')
##        mail.login('jarvisatyourservice1@gmail.com', '15963212')
##        except urllib2.URLError:
##            while True:
##                try:
##                    print "Wifi has been disconnected. Reconnecting..."
##                    login()
##                except urllib2.URLError:
##                    pass
##                else:
##                    break
##        while True:
##            bool_checker = internet_on()
##            if bool_checker == False:
##                print "Wifi has been disconnected. Reconnecting..."
##                time.sleep(60)
##            else:
##                break
        mail.select()
        #searches how many unseen "['OK']['<number>']"
        aaa, bbb = mail.search(None,'UNSEEN')
        #if the number of unseen is more than 0, read it
        if bbb != ['']:
            #set to inbox
            #log in and select the inbox
##            mail = imaplib.IMAP4_SSL('imap.gmail.com')
##            mail.login('jarvisatyourservice1@gmail.com', '15963212')
            mail.select('inbox')

            #get uids of all messages
            result, data = mail.uid('search', None, 'ALL') 
            uids = data[0].split()

            #read the lastest message
            result, data = mail.uid('fetch', uids[-1], '(RFC822)')
            m = email.message_from_string(data[0][1])
            #for i in range(1, 30):
                #typ, msg_data = mail.fetch(str(i), '(RFC822)')
            #for response_part in msg_data:
##            for response_part in data:
##                if isinstance(response_part, tuple):
            line = m.get_payload()
            
            for response_part in data:
                if isinstance(response_part, tuple):
                    msg = email.message_from_string(response_part[1])
                    for header in [ 'from' ]:
                        if msg['from'] == "\"Master James (SMS)\" <12013835929.18589976724.gKlK4mgjKB@txt.voice.google.com>":
                            #Access granted. Special commands else go to C++
                            if line == "Call me\n":
                                print "Calling..."
                                voice.call("8589976724", "2013835929", 1, None)
                            else:
                                child_conn.send("SMS--*--$%s" %line)
                    
            delete_inbox()
            cval.set_release(3)
        else:
            continue

#delete the last email

##pattern_uid = re.compile('\d+ \(UID (?P<uid>\d+)\)')
##def parse_uid(data):
##    match = pattern_uid.match(data)
##    return match.group('uid')

def delete_inbox():
    mail.select(mailbox = 'inbox', readonly = False)
    resp, items = mail.search(None, 'All')
    email_ids  = items[0].split()
    mail.store(email_ids[-1], '+FLAGS', '\\Deleted')
    mail.expunge()

#****sending from computer****
def sendsms(line):
    voice.send_sms("8589976724", line)

### Jarvis engine
def CallJarvis():
    p = Popen("C:/James/Visual Studio/C++ Program Files/Jarvis_3/Debug/Jarvis_3.exe", stdin=PIPE, stdout=PIPE, shell=True) #stderr=child_conn)
    return p
### Helper launcher for inputs
def launch_proc(target, idx, name, pipe_pair=None):
    if pipe_pair is None:
        parent_conn, child_conn = Pipe()
    else:
        parent_conn, child_conn = pipe_pair
    print "Pipe: ", parent_conn,", ",child_conn
    p = threading.Thread(target=target, args=(child_conn,) )
    p.start()
    return (p, parent_conn, idx, name)  
    
#lock = threading.Lock()
##cv = threading.Condition()
##cv_read = threading.Condition()
##cv_write = threading.Condition()

### Voice
def Voice(child_conn):
##    #Jarvis will first listen for either turn on speech or turn off speech
##    listener = speech.listenfor(["turn on speech",
##                                 "Turn on speech",
##                                 "Turn off speech",
##                                 "turn off speech",
##                                 "turnoff speech",
##                                 "Turnoff speech"], speech_callback)
    while True:
        cval.wait_for(0)
        line = speech.input()
        child_conn.send(line)
        cval.set_release(2)
    
### Console input
def Console(child_conn):
    while True:
        cval.wait_for(0)
        line = sys.stdin.readline()
        child_conn.send(line)
        cval.set_release(1)

if __name__ == '__main__':
    proc_list=[]
    
##    parent_conn, child_conn = Pipe()
##    p = threading.Thread(target=Console, args=(child_conn,) )
##    p.start()
##    proc_list.append( (p, parent_conn, "Console") )
##    
##    while True:
##        cval.wait_for(1)
##        msg = parent_conn.recv()
##        print "Terminal: ", msg,
##        cval.set_release(0)
##    p.join()
##    exit()


    # Launch Console & Voice

##    proc_list.append( launch_proc(Console, 1, "Console") )
##    proc_list.append( launch_proc(Voice, 2, "Voice") )
##    proc_list.append( launch_proc(SMS, 3, "SMS") )

    proc_map = {    1: launch_proc(Console, 1, "Console"),
                    2: launch_proc(SMS,     2, "SMS")
                    ## 3: launch_proc(Voice,   3, "Voice"),
                    
                }
    
    
    # Launch Jarvis executable
    pJarvis = CallJarvis()
    Jarvis_in = pJarvis.stdin
    Jarvis_out = pJarvis.stdout

    nLen = len(proc_map.keys())
    ii = 1
    idx = proc_map.keys()[ii]
    while True:
        idx = proc_map.keys()[ii]
        proc, parent_conn, idx, name = proc_map[idx]
        ##Waiting for response of Tony Stark
        sys.stdout.flush()
        print "cval.wait_for(", idx, ",0.5)..."
        cval.wait_for(idx, 0.5)
        print "==> Done!"
        if(cval.get() != idx):
            print "DEBUG: cval.get()== ", cval.get(), ", idx=", idx
            cval.release()
            ii = proc_map.keys.index(idx)
            continue
        ## Yes. Response available from my lord
        print "DEBUG: proc[",idx,"] will proceed"
        msg = parent_conn.recv()
        print  name,": ", msg
        ##(2) Send it to Jarvis
        if idx == 2: #SMS
            ##(3) Take the acceptance response from Jarvis
            Jarvis_in.write(msg)
            line = Jarvis_out.readline()
            print line
            sendsms(str(line))
        if idx == 1: #Console
            Jarvis_in.write(msg)
        if idx == 3:   #Voice
            Jarvis_in.write(msg + "\n")
        
        ##(4) Release condition variable
        print name,": ", msg," => Complete!"
        cval.set_release(idx)
        print "cval.reset_release(", idx , ") successful"
        ii = (ii + 1) % nLen
            
        
