import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ConcreteBank implements Bank {
// TODO
	private HashMap<String, Integer> balances = new HashMap<>();
	private HashMap<String, ReentrantLock> locks = new HashMap<>();
	
	@Override
	public boolean addAccount(String accountID, Integer initBalance) {
		synchronized (balances) {
			if (balances.containsKey(accountID))
				return false;	
			
			balances.put(accountID, initBalance);			
		}
		synchronized (locks) {
			locks.put(accountID, new ReentrantLock());			
		}
		return true;
	}
	
	@Override
	public boolean deposit(String accountID, Integer amount) {
		synchronized (balances) {
			if (!balances.containsKey(accountID))
				return false;
		}
		Lock lock;
		synchronized (locks) {
			lock = locks.get(accountID);
		}
		synchronized (lock) {
			Integer bal = balances.get(accountID);			
			bal += amount;
			balances.put(accountID, bal);
			lock.notifyAll();
			return true;			
		}
	}
	
	@Override
	public boolean withdraw(String accountID, Integer amount, long timeoutMillis) {
		synchronized (balances) {
			if (!balances.containsKey(accountID))
				return false;			
		}
		Lock lock;
		synchronized (locks) {
			lock = locks.get(accountID);
		}
		synchronized (lock) {
			if (balances.get(accountID) >= amount)
			{
				Integer bal = balances.get(accountID);
				bal -= amount;
				balances.put(accountID, bal);
				return true;
			}
			else
			{
				try 
				{
					lock.wait(timeoutMillis);
				}
				catch (InterruptedException ex) {
					ex.printStackTrace();
				}
				
				if (balances.get(accountID) >= amount)
				{
					Integer bal = balances.get(accountID);
					bal -= amount;
					balances.put(accountID, bal);
					return true;
				}
			}
			return false;			
		}
	}
	
	@Override
	public boolean transfer(String srcAccount, String dstAccount, Integer amount, long timeoutMillis) {
		if (withdraw(srcAccount, amount, timeoutMillis))
		{
			if (deposit(dstAccount, amount))
				return true;
		}
		return false;
	}
	
	@Override
	public Integer getBalance(String accountID) {
		synchronized (balances) {
			if (!balances.containsKey(accountID))
				return 0;
			return balances.get(accountID);			
		}
	}
	
	@Override
	public void doLottery(ArrayList<String> accounts, Miner miner) {
		ArrayList<Thread> threads = new ArrayList<>();
		for (String acc : accounts)
		{
			threads.add(new Thread(() -> deposit(acc, miner.mine(acc))));
		}
		for (Thread t : threads)
			t.start();
		for (Thread t : threads)
		{
			try {
				t.join();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}
	
	
}
