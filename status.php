<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Check Point Tracker</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #e6f2f9;
            margin: 0;
            padding: 20px;
        }
        
        .container {
            max-width: 1200px;
            margin: 0 auto;
        }
        
        h1 {
            color: #1d3e66;
            font-size: 48px;
            margin-bottom: 30px;
        }
        
        .content-wrapper {
            display: flex;
            gap: 30px;
        }
        
        .profile-card {
            background-color: white;
            border: 3px solid #1d3e66;
            border-radius: 10px;
            padding: 20px;
            width: 400px;
            box-sizing: border-box;
        }
        
        .profile-header {
            display: flex;
            align-items: center;
            margin-bottom: 20px;
        }
        
        .profile-icon {
            width: 80px;
            height: 80px;
            background-color: #6d9ecc;
            border-radius: 5px;
            margin-right: 15px;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        
        .profile-icon-inner {
            width: 40px;
            height: 40px;
            background-color: #1d3e66;
            border-radius: 50%;
            position: relative;
        }
        
        .profile-icon-inner::after {
            content: "";
            position: absolute;
            width: 60px;
            height: 30px;
            background-color: #1d3e66;
            border-radius: 30px 30px 0 0;
            bottom: -20px;
            left: -10px;
        }
        
        .profile-name {
            color: #1d3e66;
            font-size: 24px;
            font-weight: bold;
            margin: 0;
        }
        
        .profile-id {
            color: #1d3e66;
            font-size: 24px;
            font-weight: bold;
            margin: 0;
        }
        
        .location-info {
            color: #1d3e66;
            font-size: 22px;
            margin: 15px 0;
        }
        
        .divider {
            height: 1px;
            background-color: #1d3e66;
            margin: 20px 0;
        }
        
        .monitoring-title {
            color: #e67300;
            font-size: 22px;
            font-weight: bold;
            margin: 20px 0;
        }
        
        .monitoring-item {
            color: #1d3e66;
            font-size: 22px;
            margin: 15px 0;
        }
        
        .track-container {
            position: relative;
            flex-grow: 1;
        }
        
        .track {
            position: relative;
            background-color: #6d9ecc;
            border: 5px solid #1d3e66;
            border-radius: 30px;
            height: 600px;
            width: 100%;
            display: flex;
            flex-direction: column;
        }
        
        .mine-status {
            background-color: #a8c4e2;
            border-bottom: 5px solid #1d3e66;
            border-radius: 25px 25px 0 0;
            padding: 20px;
            text-align: center;
            margin-bottom: 20px;
        }
        
        .mine-status-text {
            color: #1d3e66;
            font-size: 36px;
            font-weight: bold;
            margin: 0;
        }
        
        .checkpoint-grid {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            grid-template-rows: repeat(2, 1fr);
            gap: 30px;
            padding: 40px;
            height: 100%;
            position: relative;
        }
        
        .checkpoint {
            width: 100px;
            height: 100px;
            background-color: white;
            border: 3px solid #1d3e66;
            border-radius: 50%;
            display: flex;
            justify-content: center;
            align-items: center;
            font-size: 36px;
            font-weight: bold;
            color: #1d3e66;
            position: relative;
            z-index: 2;
        }
        
        .checkpoint-1 {
            justify-self: center;
            align-self: center;
        }
        
        .checkpoint-2 {
            justify-self: center;
            align-self: center;
        }
        
        .checkpoint-3 {
            justify-self: center;
            align-self: center;
        }
        
        .checkpoint-4 {
            justify-self: center;
            align-self: center;
        }
        
        .checkpoint-5 {
            justify-self: center;
            align-self: center;
            grid-row: 2;
            grid-column: 1 / 3;
        }
        
        .active {
            background-color: #4CAF50;
            color: white;
        }
        
        /* Connecting lines */
        .connecting-lines {
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            z-index: 1;
            pointer-events: none;
        }
        
        .line {
            position: absolute;
            background-color: #1d3e66;
            z-index: 1;
        }
        
        /* Line from 1 to 2 (horizontal) */
        .line-1-2 {
            height: 4px;
            width: calc(50% - 50px);
            top: 25%;
            left: 25%;
            transform: translateX(50px);
        }
        
        /* Line from 2 to 3 (vertical) */
        .line-2-3 {
            width: 4px;
            height: calc(50% - 50px);
            top: 25%;
            right: 25%;
            transform: translateY(50px);
        }
        
        /* Line from 3 to 4 (horizontal) */
        .line-3-4 {
            height: 4px;
            width: calc(50% - 50px);
            bottom: 25%;
            right: 25%;
            transform: translateX(-50px);
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Check Point Tracker</h1>
        
        <div class="content-wrapper">
            <!-- Profile Card -->
            <div class="profile-card">
                <div class="profile-header">
                    <div class="profile-icon">
                        <div class="profile-icon-inner"></div>
                    </div>
                    <div>
                        <p class="profile-name" id="userName">Name: Loading...</p>
                        <p class="profile-id" id="userId">EID: Loading...</p>
                    </div>
                </div>
                
                <p class="location-info" id="userLocation">Blood Group: Loading...</p>
                <p class="location-info" id="userBlock">Current Block: Loading...</p>
                
                <div class="divider"></div>
                
                <p class="monitoring-title">Current Block Monitoring</p>
                
                <p class="monitoring-item" id="temperature">Temperature: Loading...</p>
                <p class="monitoring-item" id="humidity">Humidity: Loading...</p>
                <p class="monitoring-item" id="oxygenLevel">Oxygen Level: Loading...</p>
            </div>
            
            <!-- Track Section -->
            <div class="track-container">
                <div class="track">
                    <div class="mine-status">
                        <p class="mine-status-text">Mine Location</p>
                    </div>
                    
                    <div class="checkpoint-grid">
                        <!-- Connecting lines -->
                        <div class="connecting-lines">
                            <div class="line line-1-2"></div>
                            <div class="line line-2-3"></div>
                            <div class="line line-3-4"></div>
                        </div>
                        
                        <div class="checkpoint checkpoint-1" id="cp1">1</div>
                        <div class="checkpoint checkpoint-2" id="cp2">2</div>
                        <div class="checkpoint checkpoint-4" id="cp4">4</div>
                        <div class="checkpoint checkpoint-3" id="cp3">3</div>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <script>
    let activeSectors = new Set();
    let lastCardId = null;
    let lastLocation = null;

    async function fetchData() {
        const db = {
            "0x1": {
                "userName": "JOHN DOE",
                "userId": "25",
                "blood": "B+ve",
            }
        };

        try {
            let data = await fetch('http://192.168.1.29/data').then(res => res.json());

            const oxygenLevel = 21;

            // Find the current block and user
            let currentUser = null;
            let currentBlock = null;

            // Check if there are any users in any block
            for (const [block, users] of Object.entries(data.blocks)) {
                if (users.length > 0) {
                    currentBlock = block;
                    currentUser = db[users[0]];
                    break;
                }
            }

            if (!currentUser || !currentBlock) {
                // No active users found, display "Out of Mine" status
                document.getElementById('userName').innerText = "Name: Out of Mine";
                document.getElementById('userId').innerText = "EID: N/A";
                document.getElementById('userLocation').innerText = "Blood Group: N/A";
                document.getElementById('userBlock').innerText = "Current Block: Out of Mine";
                document.getElementById('temperature').innerText = "Temperature: N/A";
                document.getElementById('humidity').innerText = "Humidity: N/A";
                document.getElementById('oxygenLevel').innerText = "Oxygen Level: N/A";

                // Reset all checkpoints
                document.querySelectorAll('.checkpoint').forEach(cp => {
                    cp.classList.remove('active');
                });

                // Clear active sectors
                activeSectors.clear();
                return; // Exit the function early
            }

            // Update profile information
            document.getElementById('userName').innerText = `Name: ${currentUser.userName}`;
            document.getElementById('userId').innerText = `EID: ${currentUser.userId}`;
            document.getElementById('userLocation').innerText = `Blood Group: ${currentUser.blood}`;
            document.getElementById('userBlock').innerText = `Current Block: ${currentBlock}`;

            // Environmental data updates
            document.getElementById('temperature').innerText = `Temperature: ${data.temperature}`;
            document.getElementById('humidity').innerText = `Humidity: ${data.humidity}`;
            document.getElementById('oxygenLevel').innerText = `Oxygen Level: ${oxygenLevel}%`;

            // Map blocks to checkpoints
            const locationMap = {
                "1": "cp1",
                "2": "cp2",
                "3": "cp3",
                "4": "cp4"
            };

            const cpId = locationMap[currentBlock];
            if (cpId) {
                // Reset all checkpoints
                document.querySelectorAll('.checkpoint').forEach(cp => {
                    cp.classList.remove('active');
                });

                // Activate current checkpoint
                const cp = document.getElementById(cpId);
                if (cp) {
                    cp.classList.add('active');
                }

                // Store active sector
                activeSectors.clear();
                activeSectors.add(currentBlock);
            }

            // Update last known values
            lastCardId = currentUser.userId;
            lastLocation = currentBlock;

        } catch (err) {
            console.error("Error fetching data:", err);
            // Update UI with error state
            document.getElementById('userName').innerText = "Name: Error";
            document.getElementById('userId').innerText = "EID: Error";
            document.getElementById('userLocation').innerText = "Blood Group: Error";
            document.getElementById('userBlock').innerText = "Current Block: Error";
            document.getElementById('temperature').innerText = "Temperature: Error";
            document.getElementById('humidity').innerText = "Humidity: Error";
            document.getElementById('oxygenLevel').innerText = "Oxygen Level: Error";

            // Reset all checkpoints
            document.querySelectorAll('.checkpoint').forEach(cp => {
                cp.classList.remove('active');
            });

            // Clear active sectors
            activeSectors.clear();
        }
    }

    // Initial data population
    document.addEventListener('DOMContentLoaded', () => {
        // Fetch initial data
        fetchData();
        // Periodically fetch data
        setInterval(fetchData, 1000);
    });
</script>
</body>
</html>