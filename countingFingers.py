import cv2
import mediapipe as mp
import time
import serial 


def isBetween(lim1, lim2, check):
    if check <= lim1 and check >= lim2:
        return True
    if check <= lim2 and check >= lim1:
        return True
    return False

def check_scissors(lm):
    return not ((isBetween(lm[0].x, lm[6].x, lm[8].x) or isBetween(lm[0].y, lm[6].y, lm[8].y)) and (isBetween(lm[0].x, lm[10].x, lm[12].x) or isBetween(lm[0].y, lm[10].y, lm[12].y))) and ((isBetween(lm[0].x, lm[14].x, lm[16].x) or isBetween(lm[0].y, lm[14].y, lm[16].y)) and (isBetween(lm[0].x, lm[18].x, lm[20].x) or isBetween(lm[0].y, lm[18].y, lm[20].y)))  #index finger and middle finger straight and others bend

def check_rock(lm):
    return (isBetween(lm[0].x, lm[6].x, lm[8].x) or isBetween(lm[0].y, lm[6].y, lm[8].y)) and (isBetween(lm[0].x, lm[10].x, lm[12].x) or isBetween(lm[0].y, lm[10].y, lm[12].y)) and ((isBetween(lm[0].x, lm[14].x, lm[16].x) or isBetween(lm[0].y, lm[14].y, lm[16].y)) and (isBetween(lm[0].x, lm[18].x, lm[20].x) or isBetween(lm[0].y, lm[18].y, lm[20].y))) # fingers bend

def check_paper(lm):
    #return not (isBetween(lm[0].x, lm[6].x, lm[8].x) or isBetween(lm[0].y, lm[6].y, lm[8].y)) and not (isBetween(lm[0].x, lm[10].x, lm[12].x) or isBetween(lm[0].y, lm[10].y, lm[12].y)) and not (isBetween(lm[0].x, lm[14].x, lm[16].x) or isBetween(lm[0].y, lm[14].y, lm[16].y)) and not (isBetween(lm[0].x, lm[18].x, lm[20].x) or isBetween(lm[0].y, lm[18].y, lm[20].y))  #index finger and middle finger straight and others bend
    return not ((isBetween(lm[0].x, lm[14].x, lm[16].x) or isBetween(lm[0].y, lm[14].y, lm[16].y)) and (isBetween(lm[0].x, lm[18].x, lm[20].x) or isBetween(lm[0].y, lm[18].y, lm[20].y)))  

def main():
    cap = cv2.VideoCapture(0)

    arduino = serial.Serial(port='COM6', baudrate=9600, timeout=.1) 
    mpHands = mp.solutions.hands
    hands = mpHands.Hands(static_image_mode=False, max_num_hands=1, min_detection_confidence=0.5, min_tracking_confidence=0.5)

    mpDraw = mp.solutions.drawing_utils
    pTime = 0
    ctime = 0

    current_move = -1

    while True:
        success, img = cap.read()
        imgRgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        results = hands.process(imgRgb)
        if results.multi_hand_landmarks:
            for handLms in results.multi_hand_landmarks:
                lms = []
                for id, lm in enumerate(handLms.landmark):
                    h, w, c = img.shape
                    cx, cy = int(lm.x * w), int(lm.y * h)
                    lms.append(lm)

                mpDraw.draw_landmarks(img, handLms, mpHands.HAND_CONNECTIONS)
                
                #send data to arduino
                # 0 = unknown | 1 = rock | 1 = rock | 2 = scissors | 3 = paper
                if check_rock(lms) and current_move != 1:
                    arduino.write(b'1\n')
                    current_move = 1
                    print(f"rock")
                elif check_scissors(lms) and current_move != 2:
                    arduino.write(b'2\n')
                    current_move = 2
                    print("scissors")
                elif check_paper(lms) and current_move != 3:
                    arduino.write(b'3\n')
                    current_move = 3
                    print("paper")


        ctime = time.time()
        fps = 1/(ctime-pTime)
        pTime = ctime
        cv2.putText(img, str(int(fps)),(10,50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 2)

        cv2.imshow("Image", img)
        cv2.waitKey(1)


main()