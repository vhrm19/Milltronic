<!DOCTYPE html>
<html>

<head>
	<title>Milltronic</title>
	<script src="js/plotly-2.16.1.min.js"></script>
	<meta charset="UTF-8">
</head>

<style>
	.centered {
		text-align: center;
		margin: 40px auto;
	}
</style>

<body>

	<center>
		<img height="25%" width="25%" src="img/logo.png">
		<div height="25%" width="25%" id="tester"></div>
	</center>

</body>

<script>
	function plotarDados() {
		fetch("https://sheets.googleapis.com/v4/spreadsheets/1AMiAq1DkiQGqhK0I29rX9Kg8Z9quhrj3iUAKV7SVkuE/values/Log1!A1:B?alt=json&key=AIzaSyBt_1Ky-bdPwq6laNEOpHohxnfwAp5Uekk").then(res => {
			res.json().then(data => {
				TESTER = document.getElementById('tester');
				const yourData = data["values"]
				console.log(data["values"])
				x = []
				y = []
				for (let i = 0; i < yourData.length; i++) {
					x.push(yourData[i][0])
					y.push(yourData[i][1])
				}
				Plotly.newPlot(TESTER, [{ x: x, y: y, mode: 'markers', type: 'scatter', marker: { color: 'rgb(0, 0, 0)' } }], {
					margin: { t: 0 }
				});
			}).catch(err => console.log(err))
		})
	}
	plotarDados();
	setInterval(plotarDados, 11000);

</script>

</html>
