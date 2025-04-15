document.addEventListener("DOMContentLoaded", function () {
    function getGaugeColor(value, thresholds) {
        if (thresholds.g1 !== undefined && thresholds.g2 !== undefined &&
            thresholds.y1 !== undefined && thresholds.y2 !== undefined) {
            // For pH: Green in the middle, Yellow in transition zones, Red at extremes
            if (value >= thresholds.g1 && value <= thresholds.g2) {
                return "#4CAF50"; // Green (Good)
            }
            else if ((value >= thresholds.y1 && value < thresholds.g1) || (value > thresholds.g2 && value <= thresholds.y2)) {
                return "#FFC107"; // Yellow (Moderate)
            }
            else {
                return "#F44336"; // Red (Bad)
            }
        } else {
            return "#fff"; // white
        }
    }


    function createGauge(canvasId, value, min, max, thresholds) {
        var ctx = document.getElementById(canvasId).getContext("2d");
        var gaugeColor = getGaugeColor(value, thresholds);
        return new Chart(ctx, {
            type: "doughnut",
            data: {
                labels: ["Value"],
                datasets: [{
                    data: [value, max - value],
                    backgroundColor: [gaugeColor, "#012a4a"],
                    borderWidth: 0
                }]
            },
            options: {
                rotation: -29 * Math.PI,
                circumference: 90 * Math.PI - 101,
                cutout: "85%",
                responsive: true,
                plugins: {
                    legend: { display: false },
                    tooltip: { enabled: false },
                    minMaxLabels: {
                        min: min,
                        max: max
                    }
                },
                animation: {
                    animateRotate: false,
                    animateScale: true
                }
            },
            plugins: [{
                id: "minMaxLabels",
                afterDraw(chart) {
                    const { ctx, chartArea: { left, right, top, bottom }, options } = chart;
                    const minMaxOpts = options.plugins.minMaxLabels;
                    if (!minMaxOpts) return;

                    ctx.save();
                    ctx.fillStyle = "#012a4a";
                    ctx.font = "12px Arial";
                    ctx.textAlign = "start";

                    // Draw Min label (Left side)
                    ctx.fillText(`${minMaxOpts.min}`, left + 0, bottom - 10);

                    ctx.textAlign = "end";

                    // Draw Max label (Right side)
                    ctx.fillText(`${minMaxOpts.max}`, right - 0, bottom - 10);

                    ctx.restore();
                }
            }]
        });
    }

    // Define Thresholds for Each Sensor (Good / Moderate / Bad)
    const thresholds = {
        tds: { g1: 0, g2: 400, y1: 401, y2: 500 },
        watertemp: { g1: (10 + 50) / 2, g2: (22 + 50) / 2, y1: (-20 + 50) / 2, y2: (30 + 50) / 2 },
        ph: { g1: 6.5, g2: 8.5, y1: 3, y2: 12.5 },
        orp: { g1: (0 + 2000) / 2, g2: (500 + 2000) / 2, y1: (-1000 + 2000) / 2, y2: (1000 + 2000) / 2 },
        dos: { g1: 6, g2: 9, y1: 3, y2: 14 },
        turbidity: { g1: 0, g2: 10, y1: 10, y2: 50 },

        co: { g1: 0, g2: 11.5, y1: 200, y2: 28.6 },
        pm25: { g1: 0, g2: 30, y1: 100, y2: 60 },
        no2: { g1: 0, g2: 20, y1: 300, y2: 170 },
        o3: { g1: 0, g2: 50, y1: 75, y2: 100 },
        voc: { g1: 0, g2: 50, y1: 51, y2: 500 },
        humidity: { g1: 30, g2: 60, y1: 10, y2: 70 },
        airtemp: { g1: (-20 + 50) / 2, g2: (35 + 50) / 2 /*, y1: (-20 + 50) / 2, y2: (35 + 50) / 2 */ }
    };

    const tds_min = 0;
    const tds_max = 2000;
    const ph_min = 0;
    const ph_max = 14;
    const dos_min = 0;
    const dos_max = 14;
    const orp_min = -2000;
    const orp_max = 2000;
    const turb_min = 0;
    const turb_max = 100;
    const temp_min = -50;
    const temp_max = 50;

    const pm25_min = 0;
    const pm25_max = 100;
    const co_min = 0;
    const co_max = 70;
    const no2_min = 0;
    const no2_max = 150;
    const o3_min = 0;
    const o3_max = 200;
    const voc_min = 0;
    const voc_max = 3;
    const humi_min = 0;
    const humi_max = 100;

    // Initialize Gauges with Sample Data
    const orpGauge = createGauge("orpGauge", (238.3 + orp_max) / 2, orp_min, orp_max, thresholds.orp);
    const tdsGauge = createGauge("tdsGauge", 274.3, tds_min, tds_max, thresholds.tds);
    const phGauge = createGauge("phGauge", 7.0, ph_min, ph_max, thresholds.ph);
    const dosGauge = createGauge("dosGauge", 6.3, dos_min, dos_max, thresholds.dos);
    const turbidityGauge = createGauge("turbidityGauge", 1.6, turb_min, turb_max, thresholds.turbidity);
    const watertempGauge = createGauge("watertempGauge", (21.3 + temp_max) / 2, temp_min, temp_max, thresholds.watertemp);

    const pm25Gauge = createGauge("pm25Gauge", 0.017, pm25_min, pm25_max, thresholds.pm25);
    const coGauge = createGauge("coGauge", 0.011, co_min, co_max, thresholds.co);
    const no2Gauge = createGauge("no2Gauge", 0.018, no2_min, no2_max, thresholds.no2);
    const o3Gauge = createGauge("o3Gauge", 0.025, o3_min, o3_max, thresholds.o3);
    const vocGauge = createGauge("vocGauge", 0.212, voc_min, voc_max, thresholds.voc);
    const humidityGauge = createGauge("humidityGauge", 54, humi_min, humi_max, thresholds.humidity);
    const airtempGauge = createGauge("airtempGauge", (12 + temp_max) / 2, temp_min, temp_max, thresholds.airtemp);

    function updateGauge(gauge, newValue, max, min, thresholds) {
        let newColor = getGaugeColor(newValue, thresholds);
        gauge.data.datasets[0].data = [newValue, max - newValue];
        gauge.data.datasets[0].backgroundColor = [newColor, "#012a4a"];
        gauge.update();
    }

    // Function to create a real-time line chart
    function createLineChart(canvasId, label, min, max) {
        var ctx = document.getElementById(canvasId).getContext('2d');

        return new Chart(ctx, {
            type: 'line',
            data: {
                labels: [], // Time axis
                datasets: [{
                    label: label,
                    data: [],
                    borderColor: '#012a4a',
                    borderWidth: 1,
                    fill: false,
                    tension: 0.2,
                    color: '#012a4a'
                }]
            },
            options: {
                responsive: true,
                animation: true,
                maintainAspectRatio: false,
                plugins: {
                    legend: {
                        display: false // This hides the legend
                    }
                },

                scales: {
                    x: {
                        ticks: {
                            color: '#012a4a'
                        }
                    },
                    y: {
                        ticks: {
                            color: '#012a4a'
                        }
                    }
                }
            }

        });
    }

    // Create line charts for each sensor
    const orpChart = createLineChart("orpChart", "ORP (mV)", orp_min, orp_max);
    const tdsChart = createLineChart("tdsChart", "TDS (ppm)", tds_min, tds_max);
    const phChart = createLineChart("phChart", "pH Level", ph_min, ph_max);
    const dosChart = createLineChart("dosChart", "Dissolved Oxygen (mg/L)", dos_min, dos_max);
    const turbidityChart = createLineChart("turbidityChart", "Turbidity (NTU)", turb_min, turb_max);
    const watertempChart = createLineChart("watertempChart", "Water Temp (°C)", temp_min, temp_max);
    const pm25Chart = createLineChart("pm25Chart", "PM2.5 (mg/m³)", pm25_min, pm25_max);
    const coChart = createLineChart("coChart", "CO (mg/m³)", co_min, co_max);
    const no2Chart = createLineChart("no2Chart", "NO2 (µg/m³)", no2_min, no2_max);
    const o3Chart = createLineChart("o3Chart", "O3 (µg/m³)", o3_min, o3_max);
    const vocChart = createLineChart("vocChart", "VOC (mg/m³)", voc_min, voc_max);
    const humidityChart = createLineChart("humidityChart", "Humidity (%)", humi_min, humi_max);
    const airtempChart = createLineChart("airtempChart", "Air Temp (°C)", temp_min, temp_max);

    // Function to update a line chart
    function updateLineChart(chart, newValue) {
        let now = new Date().toLocaleTimeString();

        chart.data.labels.push(now);
        chart.data.datasets[0].data.push(newValue);

        // Keep only the last 20 data points
        if (chart.data.labels.length > 20) {
            chart.data.labels.shift();
            chart.data.datasets[0].data.shift();
        }
        chart.update();
    }


    ///////////web socket stuff//////////////////////////////

    let ws = new WebSocket("wss://" + window.location.host + "/ws");

    ws.onopen = function () {
        console.log("WebSocket connected!");
    };

    ws.onmessage = function (event) {
        try {
            let data = JSON.parse(event.data);

            if (data.tds !== undefined) {
                if (data.tds < tds_min) {
                    data.tds = tds_min;
                }
                if (data.tds > tds_max) {
                    data.tds = tds_max;
                }
                document.getElementById("tds").innerText = data.tds.toFixed(2);
                updateGauge(tdsGauge, data.tds, tds_max, tds_min, thresholds.tds);
                updateLineChart(tdsChart, data.tds);
            }
            if (data.co !== undefined) {
                if (data.co < co_min) {
                    data.co = co_min;
                }
                if (data.co > co_max) {
                    data.co = co_max;
                }
                document.getElementById("co").innerText = data.co.toFixed(2);
                updateGauge(coGauge, data.co, co_max, co_min, thresholds.co);
                updateLineChart(coChart, data.co);
            }
            if (data.ph !== undefined) {
                if (data.ph < ph_min) {
                    data.ph = ph_min;
                }
                if (data.ph > ph_max) {
                    data.ph = ph_max;
                }
                document.getElementById("ph").innerText = data.ph.toFixed(2);
                updateGauge(phGauge, data.ph, ph_max, ph_min, thresholds.ph);
                updateLineChart(phChart, data.ph);
            }
            if (data.orp !== undefined) {
                if (data.orp < orp_min) {
                    data.orp = orp_min;
                }
                if (data.orp > orp_max) {
                    data.orp = orp_max;
                }
                document.getElementById("orp").innerText = data.orp.toFixed(2);
                updateGauge(orpGauge, (data.orp + orp_max) / 2, orp_max, orp_min, thresholds.orp);
                updateLineChart(orpChart, data.orp);
            }
            if (data.dos !== undefined) {
                if (data.dos < dos_min) {
                    data.dos = dos_min;
                }
                if (data.dos > dos_max) {
                    data.dos = dos_max;
                }
                document.getElementById("dos").innerText = data.dos.toFixed(2);
                updateGauge(dosGauge, data.dos, dos_max, dos_min, thresholds.dos);
                updateLineChart(dosChart, data.dos);
            }
            if (data.turbidity !== undefined) {
                if (data.turbidity < turb_min) {
                    data.turbidity = turb_min;
                }
                if (data.turbidity > turb_max) {
                    data.turbidity = turb_max;
                }
                document.getElementById("turbidity").innerText = data.turbidity.toFixed(2);
                updateGauge(turbidityGauge, data.turbidity, turb_max, turb_min, thresholds.turbidity);
                updateLineChart(turbidityChart, data.turbidity);
            }
            if (data.voc !== undefined) {
                if (data.voc < voc_min) {
                    data.voc = voc_min;
                }
                if (data.voc > voc_max) {
                    data.voc = voc_max;
                }
                document.getElementById("voc").innerText = data.voc.toFixed(2);
                updateGauge(vocGauge, data.voc, voc_max, voc_min, thresholds.voc);
                updateLineChart(vocChart, data.voc);
            }
            if (data.pm25 !== undefined) {
                if (data.pm25 < pm25_min) {
                    data.pm25 = pm25_min;
                }
                if (data.pm25 > pm25_max) {
                    data.pm25 = pm25_max;
                }
                document.getElementById("pm25").innerText = data.pm25.toFixed(3);
                updateGauge(pm25Gauge, data.pm25, pm25_max, pm25_min, thresholds.pm25);
                updateLineChart(pm25Chart, data.pm25);
            }
            if (data.no2 !== undefined) {
                if (data.no2 < no2_min) {
                    data.no2 = no2_min;
                }
                if (data.no2 > no2_max) {
                    data.no2 = no2_max;
                }
                document.getElementById("no2").innerText = data.no2.toFixed(2);
                updateGauge(no2Gauge, data.no2, no2_max, no2_min, thresholds.no2);
                updateLineChart(no2Chart, data.no2);
            }
            if (data.o3 !== undefined) {
                if (data.o3 < o3_min) {
                    data.o3 = o3_min;
                }
                if (data.o3 > o3_max) {
                    data.o3 = o3_max;
                }
                document.getElementById("o3").innerText = data.o3.toFixed(2);
                updateGauge(o3Gauge, data.o3, o3_max, o3_min, thresholds.o3);
                updateLineChart(o3Chart, data.o3);
            }
            if (data.humidity !== undefined) {
                if (data.humidity < humi_min) {
                    data.humidity = humi_min;
                }
                if (data.humidity > humi_max) {
                    data.humidity = humi_max;
                }
                document.getElementById("humidity").innerText = data.humidity.toFixed(2);
                updateGauge(humidityGauge, data.humidity, humi_max, humi_min, thresholds.humidity);
                updateLineChart(humidityChart, data.humidity);
            }
            if (data.watertemp !== undefined) {
                if (data.watertemp < temp_min) {
                    data.watertemp = temp_min;
                }
                if (data.watertemp > temp_max) {
                    data.watertemp = temp_max;
                }
                document.getElementById("watertemp").innerText = data.watertemp.toFixed(2);
                updateGauge(watertempGauge, (data.watertemp + 50) / 2, temp_max, temp_min, thresholds.watertemp);
                updateLineChart(watertempChart, data.watertemp);
            }
            if (data.airtemp !== undefined) {
                if (data.airtemp < temp_min) {
                    data.airtemp = temp_min;
                }
                if (data.airtemp > temp_max) {
                    data.airtemp = temp_max;
                }
                document.getElementById("airtemp").innerText = data.airtemp.toFixed(2);
                updateGauge(airtempGauge, (data.airtemp + 50) / 2, temp_max, temp_min, thresholds.watertemp);
                updateLineChart(airtempChart, data.airtemp);
            }
        } catch (error) {
            console.error("Error parsing WebSocket data:", error);
        }
    };

    ws.onerror = function (error) {
        console.error("WebSocket error:", error);
    };

    ws.onclose = function () {
        console.warn("WebSocket closed. Reconnecting in 5 seconds...");
        setTimeout(connectWebSocket, 5000);  // Try reconnecting without reloading
    };
    //////////////end/////////////////////////////////////////

    setInterval(() => {
        // updateGauge(tdsGauge, Math.random() * 2000, 2000, thresholds.tds);
        // updateGauge(phGauge, Math.random() * 14, 14, thresholds.ph);
        // updateGauge(dosGauge, Math.random() * 14, 14, thresholds.dos);
        // updateGauge(airtempGauge, (Math.random() * 100) - 50, 50, thresholds.airtemp);
        // updateGauge(watertempGauge, (Math.random() * 100)-50, 50, thresholds.watertemp);

        // updateGauge(pm25Gauge, Math.random() * 1000, 1000, thresholds.pm25);
        // updateGauge(no2Gauge, Math.random() * 1000, 1000, thresholds.no2);
        // updateGauge(o3Gauge, Math.random() * 1000, 1000, thresholds.o3);
        // updateGauge(vocGauge, Math.random() * 100, 100, thresholds.voc);
        // updateGauge(humidityGauge, Math.random() * 100, 100, thresholds.humidity);

    }, 1000);
});





