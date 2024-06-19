import os
import random

def generate_mega_file(filename, n_pools, m_actions):
    actions = ["addLiquidity", "removeLiquidity", "swapDaiToEth", "swapEthToDai", "calculateArbitrage"]

    with open(filename, 'w') as f:
        # Create n pools
        for i in range(n_pools):
            dai = random.uniform(100000, 999999)
            eth = random.uniform(100000, 999999)
            f.write(f"addPool {dai:.2f} {eth:.2f}\n")
        
        f.write("Sleep 1000\n")
        
        # Perform m random actions
        for _ in range(m_actions):
            action = random.choice(actions)
            pool_id = random.randint(0, n_pools - 1)
            if action == "addLiquidity" or action == "removeLiquidity":
                amount1 = random.uniform(100, 500)
                amount2 = random.uniform(50, 250)
                f.write(f"{action} {pool_id} {amount1:.2f} {amount2:.2f}\n")
            elif action == "swapDaiToEth" or action == "swapEthToDai":
                amount1 = random.uniform(100, 500)
                f.write(f"{action} {pool_id} {amount1:.2f}\n")
            elif action == "calculateArbitrage":
                pool_id_2 = random.randint(0, n_pools - 1)
                while pool_id_2 == pool_id:
                    pool_id_2 = random.randint(0, n_pools - 1)
                f.write(f"{action} {pool_id} {pool_id_2}\n")

if __name__ == "__main__":
    script_dir = os.path.dirname(os.path.abspath(__file__))
    output_file = os.path.join(script_dir, "mega_benchmark_test.txt")
    generate_mega_file(output_file, n_pools=10, m_actions=10000)
    print(f"Generated mega benchmark test file at {output_file}")
