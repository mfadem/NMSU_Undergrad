'''

Team Blueberry
CS 485 UID
TPS: Functional Prototype Script
12/1/16

'''

from tkinter import *
import time

class App():
    def __init__(self):
        # Set up window variables
        self.window = Tk()
        self.root = Frame(self.window, height=150,width=30)
        self.window.title('Blueberry Productivity')

        '''
        Text Entry
        '''
        self.text = Text(self.window)  # Main text window
        self.text.config(wrap=WORD)
        self.wcLabel = Label(text="",width=10)

        '''
        Basic Variables
        '''
        self.wordCount = 0
        self.graphWidth = 200
        self.graphHeight = 220
        self.dayWC = [0]*31  # Day wordcount
        self.weekWC = [0]*8  # Week wordcount
        self.monthWC = [0]*1
        self.dayInterval = 1  # Current interval for logging daily word count
        self.dayScale = self.graphWidth/len(self.dayWC)
        self.dayCounter = 60000
        self.wkInterval = 1  # Current interval for logging weekly word count
        self.bg_color = "#B1D0D3"
        self.hl_color = "#46C4CF"
        self.window.config(bg=self.bg_color)

        # (Michael: Added these variables for all functions and checkboxes I added)
        self.goal_dayWC = 100
        self.goal_weekWC = 1000
        self.goal_monthWC = 10000
        self.remindClick = 0
        self.reminderInterval = 0
        self.reminderInterval2 = 0
        self.reminderInterval3 = 0
        self.c1 = IntVar()
        self.c2 = IntVar()
        self.c3 = IntVar()
        self.c4 = IntVar()
        self.c5 = IntVar()
        self.c6 = IntVar()
        self.cR1 = IntVar()
        self.cR2 = IntVar()
        self.cR3 = IntVar()
        self.cR4 = IntVar()
        self.cR5 = IntVar()
        self.cR6 = IntVar()
        self.cR7 = IntVar()
        self.cR8 = IntVar()
        self.cR9 = IntVar()

        '''
        Buttons
        '''
        # Goal Activation Button
        self.goalEdit = False
        self.button_makeGoal = Button(self.window, text = "   Goal   ", command=self.EnterGoal,state=NORMAL,bg=self.hl_color,height=3, width=10)

        # Reminder Activation Button
        self.remEdit = False
        self.button_makeRem = Button(self.window, text = "   Reminder   ", command=self.EnterRem,bg=self.hl_color,height=3, width=10)

        # Reset Graphs (Michael: Calls ClearGraphs() function)
        self.reset = Button(self.window, text = "Reset", command=self.ClearGraphs,bg=self.hl_color)

        '''
        Goal Entry Boxes
        '''
        # (Michael: Cleaned up placement of goals and added goals for week and month)
        # Day Goal Entrance Box
        self.dayGoal_label = Label(self.window,text = "Daily Word Count Goal", bg=self.bg_color)
        self.dayGoalWC = Entry(self.window,bg=self.hl_color,disabledbackground=self.bg_color)
        self.dayGoalWC.insert(0,str(self.goal_dayWC))
        self.dayGoalWC.config(state=DISABLED)

        # Week Goal Entrance Box
        self.weeklyGoal_label = Label(self.window,text = "Weekly Word Count Goal", bg=self.bg_color)
        self.weeklyGoalWC = Entry(self.window,bg=self.hl_color,disabledbackground=self.bg_color)
        self.weeklyGoalWC.insert(0,str(self.goal_dayWC))
        self.weeklyGoalWC.config(state=DISABLED)

        # Month Goal Entrance Box
        self.monthGoal_label = Label(self.window,text = "Monthly Word Count Goal", bg=self.bg_color)
        self.monthGoalWC = Entry(self.window,bg=self.hl_color,disabledbackground=self.bg_color)
        self.monthGoalWC.insert(0,str(self.goal_dayWC))
        self.monthGoalWC.config(state=DISABLED)

        '''
        Reminder Entry Boxes
        '''
        # (Michael: Reminder 3 input box and checkboxes)
        self.rem1_label = Entry(self.window,bg=self.hl_color,disabledbackground=self.bg_color)
        self.rem1_label.insert(0,"Enter Reminder")
        self.rem1_label.config(state=DISABLED)

        self.check_rem = Checkbutton(self.window, text="15", variable=self.cR1,bg=self.bg_color,command=lambda: self.CheckReminder1(1))
        self.check_rem.deselect()
        self.check_rem_1 = Checkbutton(self.window, text="30", variable=self.cR2,bg=self.bg_color,command=lambda: self.CheckReminder1(2))
        self.check_rem_1.deselect()
        self.check_rem_2 = Checkbutton(self.window, text="60", variable=self.cR3,bg=self.bg_color,command=lambda: self.CheckReminder1(3))
        self.check_rem_2.deselect()

        # (Michael: Reminder 2 input box and checkboxes)
        self.rem2_label = Entry(self.window,bg=self.hl_color,disabledbackground=self.bg_color)
        self.rem2_label.insert(0,"Enter Reminder")
        self.rem2_label.config(state=DISABLED)

        self.check_rem2 = Checkbutton(self.window, text="15", variable=self.cR4,bg=self.bg_color,command=lambda: self.CheckReminder2(1))
        self.check_rem2.deselect()
        self.check_rem2_1 = Checkbutton(self.window, text="30", variable=self.cR5,bg=self.bg_color,command=lambda: self.CheckReminder2(2))
        self.check_rem2_1.deselect()
        self.check_rem2_2 = Checkbutton(self.window, text="60", variable=self.cR6,bg=self.bg_color,command=lambda: self.CheckReminder2(3))
        self.check_rem2_2.deselect()

        # (Michael: Reminder 3 input box and checkboxes)
        self.rem3_label = Entry(self.window,bg=self.hl_color,disabledbackground=self.bg_color)
        self.rem3_label.insert(0,"Enter Reminder")
        self.rem3_label.config(state=DISABLED)

        self.check_rem3 = Checkbutton(self.window, text="15", variable=self.cR7,bg=self.bg_color,command=lambda: self.CheckReminder2(1))
        self.check_rem3.deselect()
        self.check_rem3_1 = Checkbutton(self.window, text="30", variable=self.cR8,bg=self.bg_color,command=lambda: self.CheckReminder2(2))
        self.check_rem3_1.deselect()
        self.check_rem3_2 = Checkbutton(self.window, text="60", variable=self.cR9,bg=self.bg_color,command=lambda: self.CheckReminder2(3))
        self.check_rem3_2.deselect()

        '''
        Widget Select
        '''
        # (Michael: Create labels for widgets and their respective checkboxes)
        self.WidgetLabel = Label(self.window, text="Graphs")
        self.GraphLabel = Label(self.window, text="Widgets")
        self.check1 = Checkbutton(self.window, text="Daily WC", variable=self.c1, bg=self.bg_color,command=lambda: self.CheckToggle(1))
        self.check1.deselect()
        self.check2 = Checkbutton(self.window, text="Weekly WC", variable=self.c2,bg=self.bg_color,command=lambda: self.CheckToggle(2))
        self.check2.deselect()
        self.check3 = Checkbutton(self.window, text="Monthly WC", variable=self.c3,bg=self.bg_color,command=lambda: self.CheckToggle(3))
        self.check3.deselect()
        self.check4 = Checkbutton(self.window, text="Email", variable=self.c4,bg=self.bg_color,command=lambda: self.CheckToggle(4))
        self.check4.deselect()
        self.check5 = Checkbutton(self.window, text="Words Left", variable=self.c5,bg=self.bg_color,command=lambda: self.CheckToggle(5))
        self.check5.deselect()
        self.check6 = Checkbutton(self.window, text="Media Feed", variable=self.c6,bg=self.bg_color,command=lambda: self.CheckToggle(6))
        self.check6.deselect()

        '''
        Graphs
        '''
        # (Michael: Default creation of canvas objects, they stay black here because the checktoggle function builds
        # the widgets )
        self.dayGraph = Canvas(self.window,height=self.graphHeight,width=self.graphWidth)
        self.weekGraph = Canvas(self.window,height=self.graphHeight,width=self.graphWidth)
        self.monthGraph = Canvas(self.window,height=self.graphHeight,width=self.graphWidth)

        '''
        Notification
        '''
        self.notif = Canvas(self.window, height=self.graphHeight/4, width=self.graphWidth)
        #self.notif.create_text(2,2,text = "Notification here",anchor="nw",tag="notifTag")
        self.notif.config(bg="#7EE2E4")

        '''
        Fake Tool Bar
        '''
        # (Michael: Put fake menus in corner, looks nicer there)
        self.fakeMenu = Label(self.window,text="File     Edit     View     Format     Help")
        self.fakeMenu.config(anchor="nw")

        '''
        Place in Grid
        '''
        # Main text Box
        self.text.grid(row=1,column=2,rowspan=15,columnspan=4)
        self.text.config(height=50)
        self.fakeMenu.grid(row=0,column=0,columnspan=7,sticky=NW)

        # Right Margin
        self.button_makeGoal.grid(row=1,column=6,sticky=E,padx=10)
        self.button_makeRem.grid(row=1,column=7,sticky=W,padx=10)

        # Placing input boxes and labels for goals
        self.dayGoalWC.grid(row=2,column=7,sticky=N, padx =5)
        self.dayGoal_label.grid(row=2,column=6,sticky=N, padx = 5)
        self.weeklyGoalWC.grid(row=2,column=7)
        self.weeklyGoal_label.grid(row=2,column=6)
        self.monthGoalWC.grid(row=2,column=7,sticky = S)
        self.monthGoal_label.grid(row=2,column=6, sticky = S)

        # Reminder Input Box #1 and it's checkboxes (Michael: Cleaned up reminders and added checkboxes for intervals)
        self.rem1_label.grid(row=3,column=6,sticky = N,pady=20,padx=5)
        self.check_rem.grid(row=3,column=7,sticky= NW,pady=20)
        self.check_rem_1.grid(row=3, column=7, sticky=N, pady=20)
        self.check_rem_2.grid(row=3, column=7, sticky=NE, pady=20)

        # Reminder Input Box #2 and it's checkboxes (Michael: Cleaned up reminders and added checkboxes for intervals)
        self.rem2_label.grid(row=3,column=6)
        self.check_rem2.grid(row=3,column=7,sticky= W,pady=20)
        self.check_rem2_1.grid(row=3, column=7, pady=20)
        self.check_rem2_2.grid(row=3, column=7, sticky=E, pady=20)

        # Reminder Input Box #3 and it's checkboxes (Michael: Cleaned up reminders and added checkboxes for intervals)
        self.rem3_label.grid(row=3,column=6,sticky = S,pady=2)
        self.check_rem3.grid(row=3,column=7,sticky= SW)
        self.check_rem3_1.grid(row=3, column=7, sticky=S)
        self.check_rem3_2.grid(row=3, column=7, sticky=SE)

        # Widget selector checkboxes and labels (Michael: Added checkboxes for six widgets that will appear on left side,
        # I tried to get them to line up and look nice but they just weren't having it)
        self.WidgetLabel.grid(row=4, column=6,sticky=N,pady=20)
        self.GraphLabel.grid(row=4, column=7,sticky=N,pady=20)
        self.check1.grid(row=4,column=6)
        self.check2.grid(row=4, column=6,sticky=S)
        self.check3.grid(row=5, column=6)
        self.check4.grid(row=4, column=7)
        self.check5.grid(row=4, column=7,sticky=S)
        self.check6.grid(row=5, column=7)

        # Left Margin (Michael: Added three graphs on left, placed word counter above text box, added reset buttons that
        #  will clear all of the graphs)
        self.notif.grid(row=1,column=0, padx=5,pady=5)
        self.wcLabel.grid(row=0,column=5, sticky=E)
        self.dayGraph.grid(row=2,column=0,columnspan=2,pady=5,sticky=S)
        self.weekGraph.grid(row=3, column=0,columnspan=2,pady=5)
        self.monthGraph.grid(row=4, column=0, columnspan=2,pady=5)
        self.reset.grid(row=5,column=0, sticky=N, padx=5)

        # Loops and Update Calls
        self.timerupdate()
        self.graphUpdate()
        self.notifiUpdate()
        self.root.mainloop()

    '''
    Button Functions
    '''
    def TextAnalysis(self):
        self.wordCount = 0
        for word in self.text.get("1.0",END).split(" "):
            self.wordCount += 1
        return self.wordCount

    # (Michael: Reset function calls this function which clears graphs and checkboxes)
    def ClearGraphs(self):
        self.dayGraph.delete("all")
        self.weekGraph.delete("all")
        self.monthGraph.delete("all")
        self.check1.deselect()
        self.check2.deselect()
        self.check3.deselect()
        self.check4.deselect()
        self.check5.deselect()
        self.check6.deselect()

    # (Michael: Added goals for weekly and monthly goals)
    def EnterGoal(self):
        if self.goalEdit == False:
            self.goalEdit = True
            self.dayGoalWC.config(state=NORMAL)
            self.weeklyGoalWC.config(state=NORMAL)
            self.monthGoalWC.config(state=NORMAL)
        else:
            self.goalEdit = False
            self.dayGoalWC.config(state=DISABLED)
            self.goal_dayWC = int(self.dayGoalWC.get())
            self.weeklyGoalWC.config(state=DISABLED)
            self.goal_weekWC = int(self.weeklyGoalWC.get())
            self.monthGoalWC.config(state=DISABLED)
            self.goal_monthWC = int(self.monthGoalWC.get())

    # (Michael: Took out out configs for the second text box since there are checkboxes now, added two more reminders)
    def EnterRem(self):
        if self.remEdit == False:
            self.remEdit = True
            self.rem1_label.config(state=NORMAL)
            self.rem2_label.config(state=NORMAL)
            self.rem3_label.config(state=NORMAL)
        else:
            self.remEdit = False
            self.rem1_label.config(state=DISABLED)
            self.rem2_label.config(state=DISABLED)
            self.rem3_label.config(state=DISABLED)

    # (Michael: BUILD THE WIDGETS IN THIS FUNCTION, THE CHECKBOXES CALL THIS FUNCTION!
    # I moved all the actual construction of the widgets to this function since they rely on the
    # checkboxes to display them. Also the graph names indicate which canvas the checkboxes build on
    # i.e. dayGraph = top canvas, weekGraph = middle canvas, monthGraph = bottom canvas.)
    def CheckToggle(self, b_id):
        if b_id == 1:
            self.dayGraph.create_text(2, 2, text="Daily Word Count", anchor="nw")  # Graph label
            self.dayGraph.create_rectangle(2, 20, self.graphWidth, self.graphHeight)  # Make boarder
        elif b_id == 2:
            self.weekGraph.create_text(2, 2, text="Weekly Word Count", anchor="nw")  # Graph label
            self.weekGraph.create_rectangle(2, 20, self.graphWidth, self.graphHeight)  # Make boarder
        elif b_id == 3:
            self.monthGraph.create_text(2, 2, text="Monthly Word Count", anchor="nw")  # Graph label
            self.monthGraph.create_rectangle(2, 20, self.graphWidth, self.graphHeight)  # Make boarder
        elif b_id == 4:
            self.weekGraph.create_text(2, 2, text="Email", anchor="nw")  # Graph label
            self.weekGraph.create_rectangle(2, 20, self.graphWidth, self.graphHeight)  # Make boarder
        elif b_id == 5:
            self.weekGraph.create_text(2, 2, text="Words Left Until Goal", anchor="nw")  # Graph label
            self.weekGraph.create_rectangle(2, 20, self.graphWidth, self.graphHeight)  # Make boarder
        elif b_id == 6:
            self.monthGraph.create_text(2, 2, text="Media Feed", anchor="nw")  # Graph label
            self.monthGraph.create_rectangle(2, 20, self.graphWidth, self.graphHeight)  # Make boarder

    # (Michael: Next three functions set interval variables for each of the three reminders, use these variables
    # for setting the timing for the notification)
    def CheckReminder1(self, r_id):
        if r_id == 1:
            self.reminderInterval = 15
        elif r_id == 2:
            self.reminderInterval = 30
        elif r_id == 3:
            self.reminderInterval = 60

    def CheckReminder2(self, r_id):
        if r_id == 1:
            self.reminderInterval2 = 15
        elif r_id == 2:
            self.reminderInterval2 = 30
        elif r_id == 3:
            self.reminderInterval2 = 60

    def CheckReminder3(self, r_id):
        if r_id == 1:
            self.reminderInterval3 = 15
        elif r_id == 2:
            self.reminderInterval3 = 30
        elif r_id == 3:
            self.reminderInterval3 = 60

    # Updater loop
    def timerupdate(self):
        self.TextAnalysis()
        self.wcLabel.configure(text=self.wordCount)
        self.dayWC[self.dayInterval] = self.wordCount
        self.root.after(100, self.timerupdate)

    def graphUpdate(self):
        if self.dayInterval < len(self.dayWC)-1:
            self.dayGraph.create_line((self.dayInterval-1)*self.dayScale,(1-(self.dayWC[self.dayInterval-1]/self.goal_dayWC))*self.graphHeight,self.dayInterval*(self.dayScale),(1-(self.dayWC[self.dayInterval]/self.goal_dayWC))*self.graphHeight,width=2)
            self.dayInterval += 1
        self.root.after(self.dayCounter, self.graphUpdate)

    def notifiUpdate(self):
        if self.dayInterval >= len(self.dayWC)-1: #Did not make goal
            if self.wordCount < self.goal_dayWC:
                self.notif.delete("notifTag")
                self.notif.create_text(2,2,text = ":(",anchor="nw",tag="notifTag")
                self.notif.config(relief="ridge",bg="#7EE2E4",bd=1)
            else:
                self.notif.delete("notifTag")
                self.notif.create_text(2,2,text = ":)",anchor="nw",tag="notifTag")
                self.notif.config(relief="ridge",bg="#7EE2E4",bd=1)
        self.root.after(100, self.notifiUpdate)


app=App()
# (Michael: Don't need this line since the window has the mainloop)
#app.mainloop()