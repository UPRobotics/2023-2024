from tkinter import *
import os
import subprocess



GUI1 = Tk()
GUI1.title("GUI1")
GUI1.geometry("1200x800")
GUI1.resizable(False, False)

Titulo = Label(GUI1, text= "Lectura de sensores")
Titulo.pack()

PlanArmArray = []

def arreglo(array):
    for i in array:
        PlanArmArray.append(i)

def simulacion():
    botons = []
    objects = []
    sensores = []

    def destroy1():
        if(len(objects)>0):
            for object in objects:
                object.destroy()

    def destroy2():
        if(len(botons)>0):
            for boton in botons:
                boton.destroy()

    def boton1():
        destroy2()
        Botones = Frame(GUI1, bg = "lightblue")
        Botones.place(x = 1100, y = 10, width = 100, height = 50)

        Btn1 = Button(Botones, text = "Btn", bg = "red")
        Btn1.pack(pady = 1)

        botons.append(Botones)

    def boton2():
        destroy2()
        Botones = Frame(GUI1, bg = "lightblue")
        Botones.place(x = 1100, y = 10, width = 100, height = 50)

        Btn2 = Button(Botones, text = "Btn", bg = "lightblue")
        Btn2.pack(pady = 1)

        botons.append(Botones)


    def boton3():
        destroy2()
        Botones = Frame(GUI1, bg = "lightblue")
        Botones.place(x = 1100, y = 10, width = 100, height = 50)

        Btn3 = Button(Botones, text = "Btn", bg = "lightgreen")
        Btn3.pack(pady = 1)

        botons.append(Botones)

    def changebtn():
        b1 = Button(GUI1, text = "boton1", bg = "red", command = boton1)
        b1.place(x = 1100, y = 80, width=100, height=50)

        b2 = Button(GUI1, text = "boton2", bg = "red", command = boton2)
        b2.place(x = 1100, y = 140, width=100, height=50)

        b3 = Button(GUI1, text = "boton3", bg = "red", command = boton3)
        b3.place(x = 1100, y = 200, width=100, height=50)


    def SensoresFR():
        Gaz = Frame(GUI1, bg = "lightblue")
        Gaz.place(x = 10, y = 10, width = 500, height = 400)

        TlGaz = Label(Gaz, text = "Lectura de Sensores", bg = "lightblue")
        TlGaz.pack()

    def RobotFR():
        destroy1()
        Robot = Frame(GUI1, bg = "lightblue")
        Robot.place(x = 10, y = 430, width = 500, height = 200)

        TlRobot = Label(Robot, text = "Movimiento Robot", bg = "lightblue")
        TlRobot.pack()

        objects.append(Robot)

    def BrazoFR():
        destroy1()

        def MoverBrazo():
            nada = 0

        def SimularBrazo():
            nada = 0

        Brazo = Frame(GUI1, bg = "lightblue")
        Brazo.place(x = 10, y = 430, width = 500, height = 200)

        TlBrazo = Label(Brazo, text = "Movimiento Brazo", bg = "lightblue")
        TlBrazo.pack()

        def xchange(int):
            x = str(SliderX.get())
            return x

        def ychange(int):
            y = str(SliderY.get())
            return y

        def zchange(int):
            z = str(SliderZ.get())
            return z

        Brazo2 = Frame(GUI1, bg = "lightblue")
        Brazo2.place(x = 10, y = 650, width = 500, height = 100)

        SliderX = Scale(Brazo, from_ = -700, to = 700, orient = HORIZONTAL, command = xchange, length = 300)
        SliderX.pack(pady = 8)

        SliderY = Scale(Brazo, from_ = -700, to = 700, orient = HORIZONTAL, command = ychange, length = 300)
        SliderY.pack(pady = 8)

        SliderZ = Scale(Brazo, from_ = 0, to = 700, orient = HORIZONTAL, command = zchange, length = 300)
        SliderZ.pack(pady = 8)

        Simular = Button(Brazo2, text = "Simular", command = SimularBrazo(xchange, ychange, zchange))
        Mover = Button(Brazo2, text = "Mover", command = MoverBrazo)
        Simular.place(relx = .15, rely = 0.25, relwidth = .25, relheight = .5)
        Mover.place(relx = .60, rely = 0.25, relwidth = .25, relheight = .5)

        objects.append(Brazo)
        objects.append(Brazo2)
    
    def CLawFR():
        destroy1()
        ClawMode = Frame(GUI1, bg = "black")
        ClawMode.place(x = 10, y = 650, width = 500, height = 100)

        ClawBtn1 = Button(ClawMode, text = "Garra Cerrada", command = OpClaw)
        ClawBtn2 = Button(ClawMode, text = "Garra abierta", command = ClClaw)
        ClawBtn3 = Button(ClawMode, text = "Girar Garra", command = RtClaw)
        ClawBtn1.place(relx = 0.02, rely = 0.2, relwidth = 0.3, relheight = 0.6)
        ClawBtn2.place(relx = 0.35, rely = 0.2, relwidth = 0.3, relheight = 0.6)
        ClawBtn3.place(relx = 0.68, rely = 0.2, relwidth = 0.3, relheight = 0.6)

        def ask_quit():
            GUI1.destroy()

        GUI1.protocol("Delete Window", ask_quit)

        objects.append(ClawMode)

    def MovimientoFR():
        Mov = Frame(GUI1, bg = "lightblue")
        Mov.place(x = 600, y = 430, width = 200, height = 300)

        TlMov = Label(Mov, text = "Movimiento", bg = "lightblue")
        TlMov.pack()

        ClawBtn1 = Button(Mov, text = "Robot", command = RobotFR)
        ClawBtn2 = Button(Mov, text = "Brazo", command = BrazoFR)
        ClawBtn3 = Button(Mov, text = "Garra", command = CLawFR)
        ClawBtn1.place(relx = 0.2, rely = 0.15, relwidth = 0.6, relheight = 0.25)
        ClawBtn2.place(relx = 0.2, rely = 0.42, relwidth = 0.6, relheight = 0.25)
        ClawBtn3.place(relx = 0.2, rely = 0.69, relwidth = 0.6, relheight = 0.25)


    def Launcher():
        cd1 = "cd"
        subprocess.run(cd1, shell=True)
        cd2 = "cd .."
        subprocess.run(cd2, shell=True)
        cd = "ls"
        subprocess.run(cd, shell=True)
        source = "source install/local_setup.bash"
        subprocess.run(source, shell=True)
        launch1 = "ros2 launch ixnaminki_description display.launch.py"
        subprocess.run(launch1, shell=True)

    def Simular(x,y,z):
        cd1 = "cd"
        subprocess.run(cd1, shell=True)
        cd2 = "cd .."
        subprocess.run(cd2, shell=True)
        cd = "ls"
        subprocess.run(cd, shell=True)
        source = "source install/local_setup.bash"
        subprocess.run(source, shell=True)
        launch1 = "ros2 run ixnaminki2_remote plan_client %d" %(x,y,z)
        subprocess.run(launch1, shell=True)

    def OpClaw():
        a = a
    
    def ClClaw():
        a = a
    
    def RtClaw():
        a = a
    

    def otros():
        Dado = Button(GUI1, text = "Dado", bg = "Gray")
        Dado.place(x = 530, y = 20, width = 50, height = 50)

        Launcher1 = Button(GUI1, text = "LAUNCHER", command = Launcher)
        Launcher1.place(x = 530, y = 80, width = 150, height = 150)

        #RbImage = PhotoImage(file = 'robot.png')
        #lblim1 = Label(GUI1, image = RbImage)
        #lblim1.place(x = 700, y = 500, width = 200, height = 500)

    SensoresFR()
    MovimientoFR()
    changebtn()
    otros()

simulacion()

GUI1.mainloop()
