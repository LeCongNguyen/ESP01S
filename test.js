
fetch('./test.php', {
    mode: "no-cors"
})
    .then((response) => { return (response.json()) })
    .then((data) => console.log(data));
