import torch
import torch.nn as nn
from torchvision import models, transforms
from PIL import Image
import cv2
import requests
import time
import pickle
import numpy as np
import joblib

BLYNK_AUTH = 'Yvp5PySneStyQHzN01Q0aZOxrEV2Y7Cj'

# Load your ML model once at the start
model = joblib.load('/home/pi/Desktop/Cognitive.pkl')

def get_blynk_data():
    BASE = "https://blynk.cloud/external/api/get"
    gas = requests.get(f"{BASE}?token={BLYNK_AUTH}&v0").text
    temp = requests.get(f"{BASE}?token={BLYNK_AUTH}&v1").text
    hum = requests.get(f"{BASE}?token={BLYNK_AUTH}&v2").text
    return int(gas), float(temp), float(hum)
while True:
    try:
        gas, temp, hum = get_blynk_data()

        print(temp,hum,gas)


        model_input = np.array([[temp, hum, gas]])
        result = model.predict(model_input)
        print("Model Prediction:", result)
        time.sleep(2)
        if result[0]==0:
            print('Fire Detected')
            device = torch.device('cpu')
            model = models.resnet18(pretrained=False)
            num_ftrs = model.fc.in_features
            model.fc = nn.Linear(num_ftrs, 2)  #num of classes
            model.load_state_dict(torch.load('/home/pi/Desktop/Fire_detection_model.pth', map_location=device))
            model.to(device)
            model.eval()

            transform = transforms.Compose([
                transforms.Resize((224, 224)),  
                transforms.ToTensor(),
                transforms.Normalize(mean=[0.485, 0.456, 0.406],
                                    std=[0.229, 0.224, 0.225])
            ])

            #open the camera
            cap = cv2.VideoCapture(0)

            if not cap.isOpened():
                print("❌ fail in camera opening")
                exit()

            while True:
                ret, frame = cap.read()
                if not ret:         
                    print("❌ there is no image that taken")
                    break
                
                # PIL to opencv
                img = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
                input_tensor = transform(img).unsqueeze(0).to(device)
                
                with torch.no_grad():
                    output = model(input_tensor)
                    prob = torch.softmax(output, dim=1)
                    confidence, predicted = torch.max(prob, 1)

                label = predicted.item()
                conf = confidence.item()


                text = f'Class: {label} Conf: {conf:.2f}'
                cv2.putText(frame, text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 
                            1, (0, 255, 0), 2, cv2.LINE_AA)
                
                cv2.imshow('Real-time Prediction', frame)

                if cv2.waitKey(1) & 0xFF == ord('q'):
                    break
    except Exception as e: 
        print("Error:", e)
        time.sleep(5)


