<!DOCTYPE html>
<html>
<head>
  <title>Checkpoint Tracker</title>
  <style>
    body {
      font-family: Arial;
      text-align: center;
      padding: 40px;
      background-color: #f3f3f3;
    }
    .checkpoint {
      display: inline-block;
      width: 80px;
      height: 80px;
      margin: 20px;
      border-radius: 50%;
      background-color: gray;
      line-height: 80px;
      color: white;
      font-weight: bold;
      font-size: 18px;
      cursor: pointer;
    }
    .active {
      background-color: green !important;
    }
    .details {
      margin-top: 30px;
      font-size: 18px;
    }
  </style>
</head>
<body>

<h1>Checkpoint Road Tracker</h1>

<div id="checkpoints">
  <div class="checkpoint" id="cp1">1</div>
  <div class="checkpoint" id="cp2">2</div>
  <div class="checkpoint" id="cp3">3</div>
  <div class="checkpoint" id="cp4">4</div>
  <div class="checkpoint" id="cp5">5</div>
</div>

<div class="details" id="cardDetails">
  Waiting for data from 192.168.1.29...
</div>

<script>
  let activeSectors = new Set();
  let lastCardId = null;
  let lastLocation = null;

  async function fetchData() {
    try {
      const res = await fetch('http://192.168.1.29/data');
      if (!res.ok) throw new Error("ESP not reachable");
      const data = await res.json();

      // Check if same data as last time
      if (data.card_id === lastCardId && data.location === lastLocation) {
        return; // Don't process again
      }

      // Update display
      document.getElementById('cardDetails').innerHTML =
        `<strong>Card ID:</strong> ${data.card_id}<br>
         <strong>Name:</strong> ${data.name}<br>
         <strong>Location:</strong> ${data.location}<br>
         <strong>Block:</strong> ${data.block}`;

      const locationMap = {
        "Sector1": "cp1",
        "Sector2": "cp2",
        "Sector3": "cp3",
        "Sector4": "cp4",
        "Sector5": "cp5"
      };

      const cpId = locationMap[data.location];
      if (cpId) {
        const cp = document.getElementById(cpId);
        if (activeSectors.has(data.location)) {
          cp.classList.remove('active');
          activeSectors.delete(data.location);
        } else {
          cp.classList.add('active');
          activeSectors.add(data.location);
        }
      }

      // Update last processed
      lastCardId = data.card_id;
      lastLocation = data.location;

    } catch (err) {
      document.getElementById('cardDetails').innerText = "Unable to reach device!";
      console.error(err);
    }
  }

  setInterval(fetchData, 1000);
</script>



</body>
</html>
