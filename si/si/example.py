import tkinter as tk

def abrir_frame2():
    frame1.destroy()  # Cierra el Frame actual
    frame2 = tk.Frame(root, width=200, height=200, bg="blue")
    frame2.pack()

root = tk.Tk()
root.geometry("300x300")

frame1 = tk.Frame(root, width=200, height=200, bg="red")
frame1.pack()

boton = tk.Button(root, text="Abrir Frame 2", command=abrir_frame2)
boton.pack()

root.mainloop()