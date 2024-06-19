import os
import random

num_files = 10
output_dir = os.path.dirname(os.path.abspath(__file__))  # Get the directory of the script

actions = ["addLiquidity", "removeLiquidity", "swapDaiToEth", "swapEthToDai", "calculateArbitrage"]
max_amount = 1000
sleep_time = 1000

def generate_test_file(filename, pool_ids):
    with open(filename, 'w') as f:
        # Add pools initially
        initial_dai = random.uniform(10000, 99999)
        initial_eth = random.uniform(10000, 99999)
        f.write(f"addPool {initial_dai} {initial_eth}\n")
        
        # Sleep after creating pools
        f.write(f"Sleep {sleep_time}\n")
        
        # Generate random actions
        for _ in range(1000):
            action = random.choice(actions)
            if action == "calculateArbitrage":
                pool1 = random.choice(pool_ids)
                pool2 = random.choice(pool_ids)
                while pool2 == pool1:
                    pool2 = random.choice(pool_ids)
                f.write(f"{action} {pool1} {pool2}\n")
            else:
                pool_id = random.choice(pool_ids)
                amount1 = random.uniform(0, max_amount)
                if action in ["addLiquidity", "removeLiquidity"]:
                    amount2 = random.uniform(0, max_amount)
                    f.write(f"{action} {pool_id} {amount1} {amount2}\n")
                else:
                    f.write(f"{action} {pool_id} {amount1}\n")


if __name__ == "__main__":
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    pool_ids = list(range(num_files))
    for i in range(num_files):
        filename = os.path.join(output_dir, f"Benchmark_non_blocking_{i + 1}.txt")
        generate_test_file(filename, pool_ids)
