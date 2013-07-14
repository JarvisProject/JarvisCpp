from multiprocessing import Process, Pipe
from subprocess import Popen, PIPE
import sys
import os
import speech
import imaplib
import email
import time
import threading

### Console input
def Console():
    for line in sys.stdin:
        child_conn.send(line)
        
### Voice input        
def speech_callback(phrase, listener):
    #prints what I said
    print ": %s\n" %(phrase)
    if phrase == "turn off speech" or phrase == "Turn off speech" or phrase == "turnoff speech" or phrase == "Turnoff speech":
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
    else:
        filerequest = open("text_0.txt", "w")
        filerequest.write(phrase)
        filerequest.close()
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
#****receiving from computer****


#log in
mail = imaplib.IMAP4_SSL('imap.gmail.com')
mail.login('jarvisatyourservice1@gmail.com', '15963212')
#see how many unseen messages there are
def unsmess():
    while 1:
        mail = imaplib.IMAP4_SSL('imap.gmail.com','993')
        mail.login('jarvisatyourservice1@gmail.com', '15963212')
        mail.select()
        #searches how many unseen "['OK']['<number>']"
        aaa, bbb = mail.search(None,'UNSEEN')
        #if the number of unseen is more than 0 read it
        if bbb != ['']:
            recmess()

def recmess():
    #set to inbox
    mail = imaplib.IMAP4_SSL('imap.gmail.com')
    mail.login('jarvisatyourservice1@gmail.com', '15963212')
    mail.select('inbox')
    #read the lastest message
    #get uids of all messages
    result, data = mail.uid('search', None, 'ALL') 
    uids = data[0].split()

    result, data = mail.uid('fetch', uids[-1], '(RFC822)')
    m = email.message_from_string(data[0][1])
    if m.get_content_maintype() == 'multipart': #multipart messages only
        for part in m.walk():
            #find the attachment part
            if part.get_content_maintype() == 'multipart': continue
            if part.get('Content-Disposition') is None: continue

            #save the attachment in the program directory
            filename = part.get_filename()
            fp = open(filename, 'wb')
            fp.write(part.get_payload(decode=True))
            fp.close()
            print '%s saved!' % filename
        #somehow notify Jarvis C++ that new txtfile has been saved
        #Jarvis will read it, interpret, execute
        #then send the string to
        #sendmess(string)
    sendmess('Sir, what shall I do now')
    delete_inbox()

#delete the last email
import getpass
import re
pattern_uid = re.compile('\d+ \(UID (?P<uid>\d+)\)')
def parse_uid(data):
    match = pattern_uid.match(data)
    return match.group('uid')

def delete_inbox():
    mail.select(mailbox = 'inbox', readonly = False)
    resp, items = mail.search(None, 'All')
    email_ids  = items[0].split()
    mail.store(email_ids[-1], '+FLAGS', '\\Deleted')
    mail.expunge()

#****sending from computer****

import smtplib
server = smtplib.SMTP( "smtp.gmail.com", 587 )
server.starttls()
server.login( 'jarvisatyourservice1@gmail.com', '15963212')
def sendmess(string):
    server.sendmail( '8589976724', '8589976724@vtext.com', string )

if __name__ == '__main__':
    Popen("C:/Users/James/Desktop/Jarvis_3.exe", shell = True) #stderr=child_conn)
    listener = speech.listenfor(["turn on speech", 
                                 "Turn on speech", 
                                 "Turn off speech",
                                 "turn off speech",
                                 "turnoff speech",
                                 "Turnoff speech"], speech_callback)
    unsmess()
    
##    proc_list=[]
##    # Launch Proc1
##    #proc_list.append( launch_proc(Console, "Console") )
##    proc_list.append( launch_proc(speech_callback, "Voice",(speech_parent, speech_child)) )
##    pJarvis = CallJarvis()
##    Jarvis_in = pJarvis.stdin
##    Jarvis_out = pJarvis.stdout
##    while(1):
##        ##(1) Print out the prompt of Jarvis
##        print Jarvis_out.readline()
##        for proc, parent_conn, name in proc_list:
##            ##Waiting for response of Tony Stark
##            if parent_conn.poll(0.5):
##                ## Yes. Response available from my lord
##                msg = parent_conn.recv()
##                print "Got some message from ", name, ": ", msg
##                ##(2) Send it to Jarvis
##                Jarvis_in.write(msg + "\n")
##                ##(3) Take the acceptance response from Jarvis
##                print Jarvis_out.readline()
##                break
