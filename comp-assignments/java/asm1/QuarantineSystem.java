import java.io.*;
import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;


public class QuarantineSystem {
    public static class DashBoard {
        List<Person> People;
        List<Integer> patientNums;
        List<Integer> infectNums;
        List<Double> infectAvgNums;
        List<Integer> vacNums;
        List<Integer> vacInfectNums;

        public DashBoard(HashMap<String, Person> p_People) {
            this.People = new ArrayList<Person>(p_People.values());
            this.patientNums = new ArrayList<>(8);
            this.infectNums = new ArrayList<>(8);
            this.infectAvgNums = new ArrayList<>(8);
            this.vacNums = new ArrayList<>(8);
            this.vacInfectNums = new ArrayList<>(8);
        }

        public void runDashBoard() {
            for (int i = 0; i < 8; i++) {
                this.patientNums.add(0);
                this.infectNums.add(0);
                this.infectAvgNums.add(0.0);
                this.vacNums.add(0);
                this.vacInfectNums.add(0);
            }

            /*
             * TODO: Collect the statistics based on People
             *  Add the data in the lists, such as patientNums, infectNums, etc.
             */
            
            for (Person person : People)
            {
            	int index = ageGroupIndex(person.getAge());
            	
            	if (person.getInfectCnt() > 0)
            		patientNums.set(index, patientNums.get(index)+1);
            	
            	infectNums.set(index, infectNums.get(index) + person.getInfectCnt());            	
            	
            	if (person.getIsVac())
            	{
            		vacNums.set(index, vacNums.get(index)+1);
            		if (person.getInfectCnt() > 0)
            			vacInfectNums.set(index, vacInfectNums.get(index)+1);
            	}
            }
            
            for (int i=0; i < 8; i++)
            {
            	if (patientNums.get(i) > 0)
            	{
            		infectAvgNums.set(i, (double) infectNums.get(i) / patientNums.get(i));
            	}
            }
            
            
        }
        
        private int ageGroupIndex(int age)
        {
        	if (age < 10)
        		return 0;
        	if (age < 20)
        		return 1;
        	if (age < 30)
        		return 2;
        	if (age < 40)
        		return 3;
        	if (age < 50)
        		return 4;
        	if (age < 60)
        		return 5;
        	if (age < 70)
        		return 6;
        	
        	return 7;
        }
    }


    private HashMap<String, Person> People;
    private HashMap<String, Patient> Patients;

    private List<Record> Records;
    private HashMap<String, Hospital> Hospitals;
    private int newHospitalNum;

    private DashBoard dashBoard;

    public QuarantineSystem() throws IOException {
        importPeople();
        importHospital();
        importRecords();
        dashBoard = null;
        Patients = new HashMap<String, Patient>();
    }

    public void startQuarantine() throws IOException {
        /*
         * Task 1: Saving Patients
         */
    	
        System.out.println("Task 1: Saving Patients");
        
        /*
         * TODO: Process each record
         */
        
        for (Record record : Records)
        {
        	if (record.getStatus() == Status.Confirmed)
        		saveSinglePatient(record);
        	else if (record.getStatus() == Status.Recovered)
        		releaseSinglePatient(record);
        }
        
        exportRecordTreatment();
        
        /*
         * Task 2: Displaying Statistics
         */
        
        System.out.println("Task 2: Displaying Statistics");
        dashBoard = new DashBoard(this.People);
        dashBoard.runDashBoard();
        exportDashBoard();
    }

    /*
     * Save a single patient when the status of the record is Confirmed
     */
    public void saveSinglePatient(Record record) {
        //TODO
    	
    	Person p = People.get(record.getIDCardNo());
    	Location pLoc = p.getLoc();
    	
    	SymptomLevel level = record.getSymptomLevel();

    	Patient patient = new Patient(p, level);
    	Patients.put(p.getIDCardNo(), patient);
    	
    	// Increment the infection count
    	p.setInfectCnt(p.getInfectCnt()+1);
    	
    	// Search for an available hospital in the existing Hospitals list
    	Hospital nearestAndAvailableHospital = null;
    	for (Hospital hospital : Hospitals.values())
    	{
    		boolean hasCapacity = hospital.getCapacity().getSingleCapacity(level) - hospital.getPatients(level).size() > 0 ? true : false;
    		if (!hasCapacity)
    			continue; // Skip the hospital that does not have enough capacity
    		
    		// Reachable only when the current hospital has enough capacity
    		// Set the current hospital to be the candidate of the hospital for treatment
    		if (nearestAndAvailableHospital == null)
    			nearestAndAvailableHospital = hospital; 
    		else
				if (pLoc.getDisSquare(hospital.getLoc()) < pLoc.getDisSquare(nearestAndAvailableHospital.getLoc()))
					nearestAndAvailableHospital = hospital;
    	}
    	
    	// Create a new hospital at the person's location since there is no existing hospital available
    	if (nearestAndAvailableHospital == null)
    	{
    		newHospitalNum++;
    		nearestAndAvailableHospital = new Hospital("H-New-"+newHospitalNum, pLoc);
    		Hospitals.put(nearestAndAvailableHospital.HospitalID, nearestAndAvailableHospital);
    	}
    	
		nearestAndAvailableHospital.addPatient(patient);
		patient.setHospitalID(nearestAndAvailableHospital.HospitalID);	
		record.setHospitalID(nearestAndAvailableHospital.HospitalID);
    }

    /*
     * Release a single patient when the status of the record is Recovered
     */
    public void releaseSinglePatient(Record record) {
        //TODO
    	Patient patient = Patients.get(record.getIDCardNo());
    	Hospital hospital = Hospitals.get(patient.getHospitalID());
    	record.setHospitalID(hospital.HospitalID);
    	hospital.releasePatient(patient);
    	Patients.remove(patient.getIDCardNo());
    }

    /*
     * Import the information of the people in the area from Person.txt
     * The data is finally stored in the attribute People
     * You do not need to change the method.
     */
    public void importPeople() throws IOException {
        this.People = new HashMap<String, Person>();
        File filename = new File("data/Person.txt");
        InputStreamReader reader = new InputStreamReader(new FileInputStream(filename));
        BufferedReader br = new BufferedReader(reader);
        String line = br.readLine();
        int lineNum = 0;

        while (line != null) {
            lineNum++;
            if (lineNum > 1) {
                String[] records = line.split("        ");
                assert (records.length == 6);
                String pIDCardNo = records[0];
                int XLoc = Integer.parseInt(records[1]);
                int YLoc = Integer.parseInt(records[2]);
                Location pLoc = new Location(XLoc, YLoc);
                assert (records[3].equals("Male") || records[3].equals("Female"));
                String pGender = records[3];
                int pAge = Integer.parseInt(records[4]);
                assert (records[5].equals("Yes") || records[5].equals("No"));
                boolean pIsVac = (records[5].equals("Yes"));
                Person p = new Person(pIDCardNo, pLoc, pGender, pAge, pIsVac);
                this.People.put(pIDCardNo, p);
            }
            line = br.readLine();
        }
    }

    /*
     * Import the information of the records
     * The data is finally stored in the attribute Records
     * You do not need to change the method.
     */
    public void importRecords() throws IOException {
        this.Records = new ArrayList<>();

        File filename = new File("data/Record.txt");
        InputStreamReader reader = new InputStreamReader(new FileInputStream(filename));
        BufferedReader br = new BufferedReader(reader);
        String line = br.readLine();
        int lineNum = 0;

        while (line != null) {
            lineNum++;
            if (lineNum > 1) {
                String[] records = line.split("        ");
                assert(records.length == 3);
                String pIDCardNo = records[0];
                assert(records[1].equals("Critical") || records[1].equals("Moderate") || records[1].equals("Mild"));
                assert(records[2].equals("Confirmed") || records[2].equals("Recovered"));
                Record r = new Record(pIDCardNo, records[1], records[2]);
                Records.add(r);
            }
            line = br.readLine();
        }
    }

    /*
     * Import the information of the hospitals
     * The data is finally stored in the attribute Hospitals
     * You do not need to change the method.
     */
    public void importHospital() throws IOException {
        this.Hospitals = new HashMap<String, Hospital>();
        this.newHospitalNum = 0;

        File filename = new File("data/Hospital.txt");
        InputStreamReader reader = new InputStreamReader(new FileInputStream(filename));
        BufferedReader br = new BufferedReader(reader);
        String line = br.readLine();
        int lineNum = 0;

        while (line != null) {
            lineNum++;
            if (lineNum > 1) {
                String[] records = line.split("        ");
                assert(records.length == 6);
                String pHospitalID = records[0];
                int XLoc = Integer.parseInt(records[1]);
                int YLoc = Integer.parseInt(records[2]);
                Location pLoc = new Location(XLoc, YLoc);
                int pCritialCapacity = Integer.parseInt(records[3]);
                int pModerateCapacity = Integer.parseInt(records[4]);
                int pMildCapacity = Integer.parseInt(records[5]);
                Capacity cap = new Capacity(pCritialCapacity, pModerateCapacity, pMildCapacity);
                Hospital hospital = new Hospital(pHospitalID, pLoc, cap);
                this.Hospitals.put(pHospitalID, hospital);
            }
            line = br.readLine();
        }
    }

    /*
     * Export the information of the records
     * The data is finally dumped into RecordTreatment.txt
     * DO NOT change the functionality of the method
     * Otherwise, you may generate wrong results in Task 1
     */
    public void exportRecordTreatment() throws IOException {
        File filename = new File("output/RecordTreatment.txt");
        OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(filename));
        BufferedWriter bw = new BufferedWriter(writer);
        bw.write("IDCardNo        SymptomLevel        Status        HospitalID\n");
        for (Record record : Records) {
            //Invoke the toString method of Record.
            bw.write(record.toString() + "\n");
        }
        bw.close();
    }

    /*
     * Export the information of the dashboard
     * The data is finally dumped into Statistics.txt
     * DO NOT change the functionality of the method
     * Otherwise, you may generate wrong results in Task 2
     */
    public void exportDashBoard() throws IOException {
        File filename = new File("output/Statistics.txt");
        OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(filename));
        BufferedWriter bw = new BufferedWriter(writer);

        bw.write("AgeRange        patientNums        infectAvgNums        vacNums        vacInfectNums\n");

        for (int i = 0; i < 8; i++) {
            String ageRageStr = "";
            switch (i) {
                case 0:
                    ageRageStr = "(0, 10)";
                    break;
                case 7:
                    ageRageStr = "[70, infinite)";
                    break;
                default:
                    ageRageStr = "[" + String.valueOf(i) + "0, " + String.valueOf(i + 1) + "0)";
                    break;
            }
            String patientNumStr = String.valueOf(dashBoard.patientNums.get(i));
            String infectAvgNumsStr = String.valueOf(dashBoard.infectAvgNums.get(i));
            String vacNumsStr = String.valueOf(dashBoard.vacNums.get(i));
            String vacInfectNumsStr = String.valueOf(dashBoard.vacInfectNums.get(i));
            

            bw.write(ageRageStr + "        " + patientNumStr + "        " + infectAvgNumsStr + "        " + vacNumsStr + "        " + vacInfectNumsStr + "\n");
        }

        bw.close();
    }

    /* The entry of the project */
    public static void main(String[] args) throws IOException {
        QuarantineSystem system = new QuarantineSystem();
        System.out.println("Start Quarantine System");
        system.startQuarantine();
        System.out.println("Quarantine Finished");
    }
}
