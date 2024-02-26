import os
import tkinter as tk
from ultralytics import YOLO
import cv2
from PIL import Image, ImageTk

cap=cv2.VideoCapture(0)
ret, frame = cap.read()
H, W, _ = frame.shape

#model_path = os.path.join('.', 'runs', 'detect', 'train14', 'weights', 'last.pt')
model = YOLO("yolov8n.pt")

# Load a model
#model = YOLO(model_path)  # load a custom model

threshold = 0.5


class App:
    def __init__(self):
        self.window = tk.Tk()
        self.window.geometry("1200x520+350+100")
        self.window.title("Face Recognition")      

        self.video_source = 0  # Puedes cambiar esto si tienes múltiples cámaras
        self.frame_count = 0
        self.vid = cv2.VideoCapture(self.video_source)

        self.canvas = tk.Canvas(self.window, width=self.vid.get(cv2.CAP_PROP_FRAME_WIDTH), height=self.vid.get(cv2.CAP_PROP_FRAME_HEIGHT))
        self.canvas.pack()

        self.btn_exit = tk.Button(self.window, text="Salir", width=10, command=self.exit)
        self.btn_exit.pack(padx=10, pady=10)

        self.update()

        self.db_dir = './db'
        if not os.path.exists(self.db_dir):
            os.mkdir(self.db_dir)

        self.log_path = './log.txt'

    def exit(self):
        self.vid.release()
        self.window.destroy()

    def update(self):
        ret, frame = self.vid.read()
        self.frame = frame
        if ret:
            results = model(frame)[0]
            for result in results.boxes.data.tolist():
                x1, y1, x2, y2, score, class_id = result
                
                cv2.rectangle(frame, (int(x1), int(y1)), (int(x2), int(y2)), (0, 255, 0), 4)
                cv2.putText(frame, results.names[int(class_id)].upper(), (int(x1), int(y1 - 10)),
                            cv2.FONT_HERSHEY_SIMPLEX, 1.3, (0, 255, 0), 3, cv2.LINE_AA) 
            self.frame = frame
            self.photo = ImageTk.PhotoImage(image=Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))) 
            self.canvas.create_image(0, 0, image=self.photo, anchor=tk.NW) 


        self.window.after(50, self.update)

    def start(self):
        self.window.mainloop()

# Crea la aplicación y comienza el bucle
app = App()
app.start()
