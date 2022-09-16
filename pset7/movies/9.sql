select name from people
where id in (
    select distinct(people.id) from movies
    join stars on movies.id = stars.movie_id
    join people on stars.person_id = people.id
    where movies.year = 2004
)
order by birth asc;