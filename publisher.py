#!/usr/bin/env python3

import rospy
from std_msgs.msg import Int32
import speech_recognition as sr

def recognize_speech():
    recognizer = sr.Recognizer()

    with sr.Microphone() as source:
        print("Listening for speech...")
        recognizer.adjust_for_ambient_noise(source)
        audio = recognizer.listen(source)

    try:
        print("Recognizing speech...")
        text = recognizer.recognize_google(audio)
        print("Recognized speech:", text)
        return text
    except sr.UnknownValueError:
        print("Could not understand audio")
    except sr.RequestError as e:
        print("Could not request results; {0}".format(e))
    return None

def publish_pick_if_detected():
    nodeName = 'message_publisher'
    topicName = 'information'

    rospy.init_node(nodeName, anonymous=True)

    publisher = rospy.Publisher(topicName, Int32, queue_size=5)

    while not rospy.is_shutdown():
        text = recognize_speech()
        if text and 'go' in text.lower():
            if 'left' in text.lower():
                rospy.loginfo("going left". Publishing 1.")
                publisher.publish(1)
            elif 'right' in text.lower():
                rospy.loginfo("going right". Publishing 2.")
                publisher.publish(2)
            elif 'up' in text.lower():
                rospy.loginfo("going up". Publishing 3.")
                publisher.publish(3)
            elif 'down' in text.lower():
                rospy.loginfo("going down". publishing 4.")
                publisher.publish(4)
            elif 'manual' in text.lower():
                rospy.loginfo("activating manual mode" . publishing 5.")
                publisher.publish(5)
            else:
                rospy.loginfo("Pick detected but no specific location mentioned.")

if __name__ == '__main__':
    try:
        publish_pick_if_detected()
    except rospy.ROSInterruptException:
        pass
