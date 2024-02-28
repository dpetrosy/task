CREATE TABLE Doctor (
    ID INT AUTO_INCREMENT PRIMARY KEY,
    Name VARCHAR(255),
    Specialization VARCHAR(255)
);

CREATE TABLE Client (
    ID INT AUTO_INCREMENT PRIMARY KEY,
    Name VARCHAR(255),
    ContactNumber VARCHAR(20)
    -- Другие атрибуты клиента
);

CREATE TABLE AppointmentSlot (
    ID INT AUTO_INCREMENT PRIMARY KEY,
    StartTime TIMESTAMP,
    Duration TIME
);

CREATE TABLE Appointment (
    ID INT AUTO_INCREMENT PRIMARY KEY,
    DoctorID INT,
    ClientID INT,
    SlotID INT,
    CreationTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (DoctorID) REFERENCES Doctor(ID),
    FOREIGN KEY (ClientID) REFERENCES Client(ID),
    FOREIGN KEY (SlotID) REFERENCES AppointmentSlot(ID)
);

CREATE TABLE DoctorAvailability (
    ID INT AUTO_INCREMENT PRIMARY KEY,
    DoctorID INT,
    DayOfWeek INT,
    StartTime TIMESTAMP,
    EndTime TIMESTAMP,
    FOREIGN KEY (DoctorID) REFERENCES Doctor(ID)
);