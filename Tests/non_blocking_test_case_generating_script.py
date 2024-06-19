import random

def generate_action(pool_ids):
    actions = ["addLiquidity", "removeLiquidity", "swapDaiToEth", "swapEthToDai"]
    action = random.choice(actions)
    
    pool_id = random.choice(pool_ids)
    amount1 = round(random.uniform(1, 1000), 2)
    amount2 = round(random.uniform(1, 1000), 2) if action != "swapDaiToEth" and action != "swapEthToDai" else 0

    return f"{action} {pool_id} {amount1} {amount2}" if amount2 != 0 else f"{action} {pool_id} {amount1}"

def generate_files(num_files, num_actions_per_file):
    pool_ids = list(range(num_files))
    initial_pools = [(round(random.uniform(100000, 200000), 2), round(random.uniform(40000, 80000), 2)) for _ in range(num_files)]

    for file_index in range(num_files):
        with open(f"Benchmark_non_blocking_{file_index+1}.txt", "w") as f:
            pool_id = pool_ids[file_index]
            initial_dai, initial_eth = initial_pools[file_index]
            f.write(f"addPool {initial_dai} {initial_eth}\n")
            f.write("Sleep 1000\n")

            for _ in range(num_actions_per_file):
                action = generate_action(pool_ids)
                f.write(action + "\n")


generate_files(10, 1000)
